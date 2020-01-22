#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "GraphicsNode.h"


namespace Graphics3D {
	class Scene {
		public:

            Scene(std::shared_ptr<ShaderProgram> shaderProgram);
            ~Scene();

            void renderScene();

            static Scene genScene(std::shared_ptr<ShaderProgram> shaderProgram, int numStaticObj, int numSceneGraphs);

            void addStaticObj(std::shared_ptr<GraphicsNode> graphicsNode);

            void addScreenGraph(std::shared_ptr<GraphicsNode> graphicsNode);

            struct SceneNode
            {
                std::shared_ptr<GraphicsNode> graphicsObj;
                std::vector<std::shared_ptr<SceneNode>> children;
            };
            
        private:

            std::shared_ptr<ShaderProgram> shaderProgram;

            std::vector<std::shared_ptr<GraphicsNode>> staticScene;
            std::vector<std::shared_ptr<SceneNode>> sceneGraphs;

            void renderSceneNode(std::shared_ptr<SceneNode> node, glm::mat4 transformMatrix);

    };
} 
