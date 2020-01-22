#include "Scene.h"
#include "Cube.h"
#include "Colors.h"
#include "SimpleMaterial.h"
#include <math.h>
#include <algorithm> 
#include <stdlib.h>


namespace Graphics3D {

    Scene::Scene(std::shared_ptr<ShaderProgram> shaderProgram) {
        this->shaderProgram = shaderProgram;
    }

    Scene::~Scene() {
        
    }


    /**
     * Renders all scene objects on the screen. 
     */
    void Scene::renderScene() {
        for (int i = 0; i < this->staticScene.size(); i++) {
            this->staticScene[i].update();
            this->staticScene[i].draw();
        }
        for (int i = 0; i < this->sceneGraphs.size(); i++) {
           this->renderSceneNode(this->sceneGraphs[i], glm::mat4(1));
        }
    }


    /**
     * Generates a random scene. 
     */
    Scene* Scene::genScene(std::shared_ptr<ShaderProgram> shaderProgram, int numStaticObj, int numSceneGraphs) {
        int numObjRows =  std::max((int) ceil(sqrt(numStaticObj + numSceneGraphs)), 1);
        int maxObjSize = 5;
        int floorSize = maxObjSize * numObjRows;
        float startXPos = -floorSize/2;
        float startYPos = -floorSize/2;
        float startZPos = 0;

        std::vector<glm::vec3> emptyTiles;
        for (int i = 0; i < pow(numObjRows, 2); i++) {
            float xPos = startXPos + (maxObjSize/2) + maxObjSize * (i%numObjRows);
            float yPos = startYPos + (maxObjSize/2) + maxObjSize * floor(i/numObjRows);
            emptyTiles.push_back(glm::vec3( xPos, startZPos, yPos));
        }
        
        Scene* scene = new Scene(shaderProgram);
        std::shared_ptr<SimpleMaterial> simpleMaterialBrown(new SimpleMaterial(shaderProgram, BROWN));
        scene->addStaticObj(Cube(floorSize, 1, floorSize, simpleMaterialBrown, 
            glm::vec3(0, -0.5, 0)));

        
        for (int i = 0; i < numStaticObj; i++) {
            int index = rand() % emptyTiles.size();
            glm::vec3 position = emptyTiles[index];
            emptyTiles.erase(emptyTiles.begin() + index);
            scene->addStaticObj(genBox(shaderProgram, position, maxObjSize));
        }


        return scene;
    }


    void Scene::addStaticObj(GraphicsNode graphicsNode) {
        this->staticScene.push_back(graphicsNode);
    }


    void Scene::addScreenGraph(SceneNode sceneNode) {
        this->sceneGraphs.push_back(sceneNode);
    }


    GraphicsNode Scene::genBox(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position, float maxObjSize) {
        int numColors = 7;
        glm::vec3 colors[] = {WHITE, RED, GREEN, BLUE, YELLOW, ORANGE, BROWN};
        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, colors[rand() % numColors]));

        float width = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize));
        float hight = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize));
        float depth = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize));

        position.y = hight/2;

        return Cube(width, hight, depth, simpleMaterial, position);
    }


    /**
     * Renders a scene node and all its children with respect to the parents transform matrix. 
     */
    void Scene::renderSceneNode(SceneNode node, glm::mat4 transformMatrix) {
        node.graphicsObj.update();
        node.graphicsObj.draw(transformMatrix);
        glm::mat4 newMatrix = transformMatrix * node.graphicsObj.getM();
        for (int i = 0; i < node.children.size(); i++) {
            this->renderSceneNode(node.children[i], newMatrix);
        }
    }
}
