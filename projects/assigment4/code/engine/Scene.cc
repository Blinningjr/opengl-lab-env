#include "Scene.h"
#include "../graphicsNodes/Cube.h"
#include "../graphicsNodes/Tetrahedron.h"
#include "../utils/Colors.h"
#include "../materials/SimpleMaterial.h"
#include <math.h>
#include <algorithm> 
#include <stdlib.h>
#include <time.h> 


namespace Graphics3D {

    Scene::Scene(std::shared_ptr<ShaderProgram> shaderProgram) {
        this->shaderProgram = shaderProgram;
        srand(time(0));
    }

    Scene::~Scene() {
        delete[] &this->staticScene;
        delete[] &this->sceneGraphs;
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
        int totalObj = numStaticObj + numSceneGraphs;
        int numObjRows =  std::max((int) ceil(sqrt(totalObj)), 1);
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
            if (rand() % 100 < 50) {
                scene->addStaticObj(genBox(shaderProgram, position, maxObjSize));
            } else {
                scene->addStaticObj(genTetrahedron(shaderProgram, position, maxObjSize));
            }
            
        }

        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, WHITE));
        for (int i = 0; i < numSceneGraphs; i++) {
            int index = rand() % emptyTiles.size();
            glm::vec3 position = emptyTiles[index];
            emptyTiles.erase(emptyTiles.begin() + index);
            position.y = 1;
            scene->addScreenGraph(genDoor(shaderProgram, position, maxObjSize));
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

        float pitch = 0;
        float roll = 0;
        float yawn = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14f));

        if (rand() % 100 < 10) {
            size.x = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0f)), 0.2f);
            size.y = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0f)), 0.2f);
            size.z = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0f)), 0.2f);
            pitchSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f));
            rollSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f));;
            yawnSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f));;

            position.y = maxObjSize/2.0f;
        } else {
            size.x = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize)), 0.2f);
            size.y = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize)), 0.2f);
            size.z = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxObjSize)), 0.2f);

            position.y = size.y/2.0f;
        }

        return new Cube(size, simpleMaterial, position, pitch, roll, yawn, pitchSpeed, rollSpeed, yawnSpeed);
    }

    GraphicsNode* Scene::genTetrahedron(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position, float maxObjSize) {
        int numColors = 7;
        glm::vec3 colors[] = {WHITE, RED, GREEN, BLUE, YELLOW, ORANGE, BROWN};
        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, colors[rand() % numColors]));

        float maxSize = maxObjSize/2;

        float size = 0;

        float pitchSpeed = 0;
        float rollSpeed = 0;
        float yawnSpeed = 0;

        float pitch = 0;
        float roll = 0;
        float yawn = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14f));

        if (rand() % 100 < 10) {

            size = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)), 0.1f); 

            pitchSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f));
            rollSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f));;
            yawnSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f));;

            position.y = maxObjSize/2.0f;
        } else {
            size = std::max(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxSize)), 0.1f); 
            position.y = size/2.0f;
        }

        return new Tetrahedron(size, simpleMaterial, position, pitch, roll, yawn, pitchSpeed, rollSpeed, yawnSpeed);
    }


    Scene::SceneNode Scene::genDoor(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position, float maxObjSize) {
        int numColors = 6;
        glm::vec3 colors[] = {RED, GREEN, BLUE, YELLOW, ORANGE, BROWN};
        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, colors[rand() % numColors]));
        std::shared_ptr<SimpleMaterial> simpleMaterialWhite(new SimpleMaterial(shaderProgram, WHITE));
        
        std::vector<SceneNode> children;

        std::vector<SceneNode> doorChildren;

        struct SceneNode tetrahedronFront = {
            new Tetrahedron(0.1f, simpleMaterialWhite, glm::vec3(0, 0, 0.3), 1, 1 ,1),
        };
        doorChildren.push_back(tetrahedronFront);

        struct SceneNode tetrahedronBack = {
            new Tetrahedron(0.1f, simpleMaterialWhite, glm::vec3(0, 0, -0.3), 1, 1 ,1),
        };
        doorChildren.push_back(tetrahedronBack);
        
        struct SceneNode door = {
            new Cube(glm::vec3(1, 2, 0.2), simpleMaterial, glm::vec3(1, 0, 0), 0, 0, 0, 0.2, 2),
            doorChildren,
        };
        children.push_back(door);

        struct SceneNode wall = {
            new Cube(glm::vec3(1, 2, 0.2), simpleMaterial, glm::vec3(2, 0, 0)),
        };
        children.push_back(wall);

        float yawn = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14f)); 
        position.x -= cos(yawn);
        position.z += sin(yawn);
        struct SceneNode sceneNode = {
            new Cube(glm::vec3(1, 2, 0.2), simpleMaterial, position, yawn),
            children,
        };

        return sceneNode;
    }


    /**
     * Renders a scene node and all its children with respect to the parents transform matrix. 
     */
    void Scene::renderSceneNode(SceneNode node, glm::mat4 transformMatrix, float deltaTime) {
        node.graphicsObj->update(deltaTime);
        node.graphicsObj->draw(transformMatrix);
        glm::mat4 newMatrix = node.graphicsObj->getM();
        for (int i = 0; i < node.children.size(); i++) {
            this->renderSceneNode(node.children[i], newMatrix, deltaTime);
        }
    }
}
