#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "Material.h"
#include "Mesh.h"
#include "GraphicsNode.h"


namespace Graphics3D {
	class Cube : public GraphicsNode {
		public:

            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position);
            Cube(float width, float hight, float depth, std::shared_ptr<Material> material,  glm::vec3 position);
            Cube(float width, float hight, float depth, std::shared_ptr<Material> material,  glm::vec3 position, 
                    glm::vec3 scale, GLfloat pitch, GLfloat roll, GLfloat yawn);
            ~Cube();

        private:

            std::shared_ptr<Mesh> genMesh(float width, float hight, float depth);
        
    };
} 
