#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "GraphicsNode.h"


namespace Graphics3D {
	class Scene : public GraphicsNode {
		public:

            Scene();
            ~Scene();

            std::vector<GraphicsNode> sceneNodes;

        private:

            std::vector<GraphicsNode> genScene();
        
    };
} 
