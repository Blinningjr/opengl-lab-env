#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "Material.h"
#include "Mesh.h"
#include "Node.h"


namespace Graphics3D {
	class GraphicsNode : public Node {
		public:

            GraphicsNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,  glm::vec3 position, glm::vec3 scale, GLfloat pitch, GLfloat roll, GLfloat yawn);
            ~GraphicsNode();

            void draw();

            void update() override {}

        private:

            std::shared_ptr<Material> material;
            std::shared_ptr<Mesh> mesh;
        
    };
} 
