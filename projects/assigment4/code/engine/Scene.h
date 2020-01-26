#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "GraphicsNode.h"
#include "quadTree/QuadTreeNode.h"


namespace Graphics3D {
	class Scene {
		public:

            struct SceneNode {
                GraphicsNode* graphicsObj;
                std::vector<SceneNode> children;
            };

            Scene(std::shared_ptr<ShaderProgram> shaderProgram, float pov, float minViewDist, float maxViewDist,
                float size);
            ~Scene();

            void renderScene(glm::vec3 cameraDirection, glm::vec3 cameraPos, float deltaTime, uint frame);

            static Scene* genScene(std::shared_ptr<ShaderProgram> shaderProgram, int numStaticObj, int numSceneGraphs,
                float pov, float minViewDist, float maxViewDist);

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

            float pov;
            float minViewDist;
            float maxViewDist;

            std::shared_ptr<QuadTreeNode> quadTreeRoot;

            void renderSceneNode(SceneNode node, glm::mat4 transformMatrix, float deltaTime);

            void renderQuadTree(glm::vec3 cameraDirection, glm::vec3 cameraPos, uint frame);

    };
} 
