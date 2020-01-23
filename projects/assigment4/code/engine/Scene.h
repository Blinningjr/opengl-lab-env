#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "GraphicsNode.h"


namespace Graphics3D {
	class Scene {
		public:

            struct SceneNode {
                GraphicsNode* graphicsObj;
                std::vector<SceneNode> children;
            };

            Scene(std::shared_ptr<ShaderProgram> shaderProgram);
            ~Scene();

            void renderScene(float deltaTime);

            static Scene* genScene(std::shared_ptr<ShaderProgram> shaderProgram, int numStaticObj, int numSceneGraphs);

            void addStaticObj(GraphicsNode* graphicsNode);

            void addScreenGraph(SceneNode sceneNode);

            static GraphicsNode* genBox(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position, float maxObjSize);

            static GraphicsNode* genTetrahedron(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position, float maxObjSize);
            
            static SceneNode genDoor(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position);

            static SceneNode genSolarSystem(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 position);

        private:

            std::shared_ptr<ShaderProgram> shaderProgram;

            std::vector<GraphicsNode*> staticScene;
            std::vector<SceneNode> sceneGraphs;

            void renderSceneNode(SceneNode node, glm::mat4 transformMatrix, float deltaTime);

    };
} 
