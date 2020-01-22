#include "Scene.h"
#include "Cube.h"
#include "Colors.h"
#include "SimpleMaterial.h"


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
            this->staticScene[i]->update();
            this->staticScene[i]->draw();
        }
        for (int i = 0; i < this->sceneGraphs.size(); i++) {
           this->renderSceneNode(this->sceneGraphs[i], glm::mat4(1));
        }
    }


    /**
     * Generates a random scene. 
     */
    Scene Scene::genScene(std::shared_ptr<ShaderProgram> shaderProgram, int numStaticObj, int numSceneGraphs) {
        Scene scene = Scene(shaderProgram);
        std::shared_ptr<SimpleMaterial> simpleMaterialBrown(new SimpleMaterial(shaderProgram, BROWN));
        scene.addStaticObj(Cube(100, 100, 100, simpleMaterialBrown, glm::vec3(0, 0, 0)));
        return scene;
    }


    void Scene::addStaticObj(std::shared_ptr<GraphicsNode> graphicsNode) {
        this->staticScene.push_back(graphicsNode);
    }


    void Scene::addScreenGraph(std::shared_ptr<GraphicsNode> graphicsNode) {
        this->addScreenGraph(graphicsNode);
    }


    /**
     * Renders a scene node and all its children with respect to the parents transform matrix. 
     */
    void Scene::renderSceneNode(std::shared_ptr<SceneNode> node, glm::mat4 transformMatrix) {
        node->graphicsObj->update();
        node->graphicsObj->draw(transformMatrix);
        glm::mat4 newMatrix = transformMatrix * node->graphicsObj->getM();
        for (int i = 0; i < node->children.size(); i++) {
            this->renderSceneNode(node->children[i], newMatrix);
        }
    }
}
