#include "Scene.h"
#include "node/graphicsNodes/Cube.h"
#include "node/graphicsNodes/Tetrahedron.h"
#include "utils/Colors.h"
#include "material/SimpleMaterial.h"
#include <math.h>
#include <algorithm> 
#include <stdlib.h>
#include <time.h> 
#include "iostream"


namespace Graphics3D {


    Scene::Scene(std::shared_ptr<ShaderProgram> shaderProgram, float pov, float minViewDist, float maxViewDist,
            float size) {
        this->shaderProgram = shaderProgram;
        this->pov = pov;
        this->minViewDist = minViewDist;
        this->maxViewDist = maxViewDist;

        std::shared_ptr<QuadTreeNode> root(new QuadTreeNode(glm::vec2(0, 0), size,(uint) 0));
        this->quadTreeRoot = root;

        srand(time(0));
    }


    Scene::~Scene() {
        delete[] &this->staticScene;
        delete[] &this->sceneGraphs;
    }


    /**
     * Renders all visable scene objects. 
     */
    void Scene::renderScene(glm::vec3 cameraDirection, glm::vec3 cameraPos, float deltaTime, uint frame) {
        for (int i = 0; i < this->staticScene.size(); i++) {
            this->staticScene[i]->update(deltaTime);
            // this->staticScene[i]->draw();
        }
        this->staticScene[0]->draw();

        for (int i = 0; i < this->sceneGraphs.size(); i++) {
           this->renderSceneNode(this->sceneGraphs[i], glm::mat4(1), deltaTime);
        }

        // this->quadTreeRoot->drawAll(frame);
        this->renderQuadTree(cameraDirection, cameraPos, frame);
    }


    /**
     * Generates a random scene. 
     */
    Scene* Scene::genScene(std::shared_ptr<ShaderProgram> shaderProgram, int numStaticObj, int numSceneGraphs,
            float pov, float minViewDist, float maxViewDist) {
        int totalObj = numStaticObj + numSceneGraphs;
        int numObjRows =  std::max((int) ceil(sqrt(totalObj) + totalObj/50.0f), 1);
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
        
        Scene* scene = new Scene(shaderProgram, pov, minViewDist, maxViewDist, floorSize/2.0f);
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
            if (rand() % 100 < 50) {
                scene->addScreenGraph(genDoor(shaderProgram, position));
            } else {
                scene->addScreenGraph(genSolarSystem(shaderProgram, position));
            }
            
        }

        return scene;
    }


    /**
     * Adds a static object to the scene.
    */
    void Scene::addStaticObj(GraphicsNode* graphicsNode) {
        this->staticScene.push_back(graphicsNode);
        if (this->staticScene.size() != 1)
            this->quadTreeRoot->insertGraphicsNode(graphicsNode);
    }


    /**
     *  Adds a scene graph to the scene
    */
    void Scene::addScreenGraph(SceneNode sceneNode) {
        this->sceneGraphs.push_back(sceneNode);
    }


    /**
     * Generates a box GraphicsNode.
    */
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


    /**
     * Generates a tetrahedron GraphicsNode.
    */
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


    /**
     * Generates a door scene graph.
    */
    Scene::SceneNode Scene::genDoor(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position) {
        std::vector<glm::vec3> colors;
        colors.push_back(RED);
        colors.push_back(GREEN);
        colors.push_back(BLUE);
        colors.push_back(WHITE);
        colors.push_back(YELLOW);
        colors.push_back(ORANGE);
        colors.push_back(BROWN);

        int index = rand() % colors.size();
        std::shared_ptr<SimpleMaterial> wallMaterial(new SimpleMaterial(shaderProgram, colors[index]));
        colors.erase(colors.begin() + index);

        index = rand() % colors.size();
        std::shared_ptr<SimpleMaterial> doorMaterial(new SimpleMaterial(shaderProgram, colors[index]));
        colors.erase(colors.begin() + index);

        index = rand() % colors.size();
        std::shared_ptr<SimpleMaterial> tetrahedronMaterial(new SimpleMaterial(shaderProgram, colors[index]));
        colors.erase(colors.begin() + index);

        
        std::vector<SceneNode> children;

        std::vector<SceneNode> doorChildren;

        float pitchSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
        float rollSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
        float yawnSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;

        struct SceneNode tetrahedronFront = {
            new Tetrahedron(0.1f, tetrahedronMaterial, glm::vec3(0, 0, 0.3), 1, 1 ,1),
        };
        doorChildren.push_back(tetrahedronFront);

        pitchSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
        rollSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
        yawnSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;

        struct SceneNode tetrahedronBack = {
            new Tetrahedron(0.1f, tetrahedronMaterial, glm::vec3(0, 0, -0.3), 1, 1 ,1),
        };
        doorChildren.push_back(tetrahedronBack);
        
        struct SceneNode door = {
            new Cube(glm::vec3(1, 2.5f, 0.2), doorMaterial, glm::vec3(1, 0, 0), 0, 0, 0, 0.2, 2.5f),
            doorChildren,
        };
        children.push_back(door);

        struct SceneNode wall = {
            new Cube(glm::vec3(1, 2.5f, 0.2), wallMaterial, glm::vec3(2, 0, 0)),
        };
        children.push_back(wall);

        float yawn = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.14f)); 
        position.x -= cos(yawn);
        position.y = 1.25f;
        position.z += sin(yawn);
        struct SceneNode sceneNode = {
            new Cube(glm::vec3(1, 2.5f, 0.2), wallMaterial, position, yawn),
            children,
        };

        return sceneNode;
    }


    /**
     *  Generates a solarsystem scene object.
    */
    Scene::SceneNode Scene::genSolarSystem(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position) {
        std::vector<glm::vec3> colors;
        colors.push_back(RED);
        colors.push_back(GREEN);
        colors.push_back(BLUE);
        colors.push_back(WHITE);
        colors.push_back(YELLOW);
        colors.push_back(ORANGE);
        colors.push_back(BROWN);

        float pitchSpeed = 0;
        float rollSpeed = 0;
        float yawnSpeed = 0;
        
        std::vector<SceneNode> sunChildren;
        for (int i = 0; i < 2; i++) {

            std::vector<SceneNode> planetChildren;
            int numMoons = rand()%3;
            for (int j = 0; j < numMoons; j++) {

                int index = rand() % colors.size();
                std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, colors[index]));
                colors.erase(colors.begin() + index);

                pitchSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
                rollSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
                yawnSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;

                struct SceneNode moon = {
                    new Tetrahedron(0.1f, simpleMaterial, glm::vec3((j * 2 - 1) * 0.8f, 0, 0), pitchSpeed, rollSpeed ,yawnSpeed),
                };
                planetChildren.push_back(moon);
            }

            int index = rand() % colors.size();
            std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, colors[index]));
            colors.erase(colors.begin() + index);

            pitchSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
            rollSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
            yawnSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;

            struct SceneNode planet = {
                new Tetrahedron(0.2f, simpleMaterial, glm::vec3((i * 2 - 1) * 1.5f, 0, 0), pitchSpeed, rollSpeed ,yawnSpeed),
                planetChildren,
            };
            sunChildren.push_back(planet);
        }

        pitchSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
        rollSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;
        yawnSpeed = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0f))) - 2.5f;

        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, colors[0]));
        position.y = 2.5;
        struct SceneNode sun = {
            new Tetrahedron(0.4f, simpleMaterial, position, pitchSpeed, rollSpeed ,yawnSpeed),
            sunChildren,
        };

        return sun;
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


    /**
     *  Renders the object visable in the quadtree.
    */
    void Scene::renderQuadTree(glm::vec3 cameraDirection, glm::vec3 cameraPos, uint frame) {
        glm::vec3 direction = glm::normalize(glm::vec3(cameraDirection.x, 0, cameraDirection.z));
        float farDistance = this->maxViewDist;
        float farSize = tan(this->pov) * farDistance;
        float closeDistance = this->minViewDist;
        float closeSize = tan(this->pov) * closeDistance;
        glm::vec3 rightDirection = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));

        // cameraPos.x = 0;
        // cameraPos.y = 0;
        // cameraPos.z = 0;
        
        glm::vec3 closeLeft(cameraPos + direction * closeDistance - rightDirection * closeSize);
        glm::vec3 closeRight(cameraPos + direction * closeDistance + rightDirection * closeSize);
        glm::vec3 farLeft(cameraPos + direction * farDistance - rightDirection * farSize);
        glm::vec3 farRight(cameraPos + direction * farDistance + rightDirection * farSize);

        // glm::vec3 closeLeft(-1, 0 , -1);
        // glm::vec3 closeRight(-1, 0 , 1);
        // glm::vec3 farLeft(50, 0 , -50);
        // glm::vec3 farRight(50, 0 , 50);

        // std::cout << "\n";
        // std::cout << cameraPos.x;
        // std::cout << ":";
        // std::cout << cameraPos.z;
        // std::cout << "\n";
        // std::cout << cameraDirection.x;
        // std::cout << ":";
        // std::cout << cameraDirection.y;
        // std::cout << ":";
        // std::cout << cameraDirection.z;
        // std::cout << "\n";
        // std::cout << closeLeft.x;
        // std::cout << ":";
        // std::cout << closeLeft.z;
        // std::cout << "\n";
        // std::cout << closeRight.x;
        // std::cout << ":";
        // std::cout << closeRight.z;
        // std::cout << "\n";
        // std::cout << farLeft.x;
        // std::cout << ":";
        // std::cout << farLeft.z;
        // std::cout << "\n";
        // std::cout << farRight.x;
        // std::cout << ":";
        // std::cout << farRight.z;
        // std::cout << "\n";


        this->quadTreeRoot->drawNodes(glm::vec2(closeLeft.x, closeLeft.z),
                                        glm::vec2(closeRight.x, closeRight.z),
                                        glm::vec2(farLeft.x, farLeft.z),
                                        glm::vec2(farRight.x, farRight.z),
                                        frame);
    }
}
