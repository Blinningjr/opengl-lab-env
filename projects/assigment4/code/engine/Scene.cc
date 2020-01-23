#include "Scene.h"
#include "../graphicsNodes/Cube.h"
#include "../graphicsNodes/Tetrahedron.h"
#include "../utils/Colors.h"
#include "../materials/SimpleMaterial.h"
#include <math.h>
#include <algorithm> 
#include <stdlib.h>


namespace Graphics3D {

    Scene::Scene(std::shared_ptr<ShaderProgram> shaderProgram) {
        this->shaderProgram = shaderProgram;
    }

    Scene::~Scene() {
        delete[] &this->staticScene;
    }


    /**
     * Renders all scene objects on the screen. 
     */
    void Scene::renderScene(float deltaTime) {
        for (int i = 0; i < this->staticScene.size(); i++) {
            this->staticScene[i]->update(deltaTime);
            this->staticScene[i]->draw();
        }
        for (int i = 0; i < this->sceneGraphs.size(); i++) {
           this->renderSceneNode(this->sceneGraphs[i], glm::mat4(1), deltaTime);
        }
    }


    /**
     * Generates a random scene. 
     */
    Scene* Scene::genScene(std::shared_ptr<ShaderProgram> shaderProgram, int numStaticObj, int numSceneGraphs) {
        int numObjRows =  std::max((int) ceil(sqrt(numStaticObj + numSceneGraphs)), 1);
        int maxObjSize = 5;
        int floorSize = maxObjSize * numObjRows;
        float startXPos = -floorSize/2.0f;
        float startYPos = 0;
        float startZPos = -floorSize/2.0f;

        std::vector<glm::vec3> emptyTiles;
        for (int i = 0; i < pow(numObjRows, 2); i++) {
            float xPos = startXPos + maxObjSize/2.0f + maxObjSize * (i%numObjRows);
            float zPos = startZPos + maxObjSize/2.0f + maxObjSize * floor(i/numObjRows);
            emptyTiles.push_back(glm::vec3( xPos, startYPos, zPos));
        }
        
        Scene* scene = new Scene(shaderProgram);
        std::shared_ptr<SimpleMaterial> simpleMaterialBrown(new SimpleMaterial(shaderProgram, BROWN));
        scene->addStaticObj(new Cube(glm::vec3(floorSize, 1, floorSize), simpleMaterialBrown, 
            glm::vec3(0, -0.5, 0)));

        
        for (int i = 0; i < numStaticObj; i++) {
            int index = rand() % emptyTiles.size();
            glm::vec3 position = emptyTiles[index];
            emptyTiles.erase(emptyTiles.begin() + index);
            scene->addStaticObj(genBox(shaderProgram, position, maxObjSize));
        }


        return scene;
    }


    void Scene::addStaticObj(GraphicsNode* graphicsNode) {
        this->staticScene.push_back(graphicsNode);
    }


    void Scene::addScreenGraph(SceneNode sceneNode) {
        this->sceneGraphs.push_back(sceneNode);
    }


    GraphicsNode* Scene::genBox(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position, float maxObjSize) {
        int numColors = 7;
        glm::vec3 colors[] = {WHITE, RED, GREEN, BLUE, YELLOW, ORANGE, BROWN};
        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, colors[rand() % numColors]));

        glm::vec3 size(0);

        float pitchSpeed = 0;
        float rollSpeed = 0;
        float yawnSpeed = 0;

        if (rand() % 100 < 10) {
            size.x = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0f)), 0.2f);
            size.y = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0f)), 0.2f);
            size.z = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0f)), 0.2f);
            pitchSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
            rollSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));;
            yawnSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));;

            position.y = maxObjSize/2.0f;
        } else {
            size.x = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize)), 0.2f);
            size.y = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize)), 0.2f);
            size.z = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize)), 0.2f);

            position.y = size.y/2.0f;
        }

        return new Cube(size, simpleMaterial, position, pitchSpeed, rollSpeed, yawnSpeed);
    }

    GraphicsNode Scene::genTetrahedron(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position, float maxObjSize) {
        int numColors = 7;
        glm::vec3 colors[] = {WHITE, RED, GREEN, BLUE, YELLOW, ORANGE, BROWN};
        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, colors[rand() % numColors]));

        float maxSize = maxObjSize/2;

        float size = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxSize)), 0.1f);

        position.y = size/2;

        return Tetrahedron(size, simpleMaterial, position);
    }


    /**
     * Renders a scene node and all its children with respect to the parents transform matrix. 
     */
    void Scene::renderSceneNode(SceneNode node, glm::mat4 transformMatrix, float deltaTime) {
        node.graphicsObj.update(deltaTime);
        node.graphicsObj.draw(transformMatrix);
        glm::mat4 newMatrix = transformMatrix * node.graphicsObj.getM();
        for (int i = 0; i < node.children.size(); i++) {
            this->renderSceneNode(node.children[i], newMatrix, deltaTime);
        }
    }
}
