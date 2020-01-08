#pragma once


#include <memory>
#include <vector>


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective



namespace Graphics3D {
	class Node {
		public:

            virtual void update() = 0;
            virtual void add(std::shared_ptr<Node> node) {
                L.push_back(node);
            };

        protected:

            glm::mat4 M; 

            std::vector<std::shared_ptr<Node>> L;

    };
} 
