#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "Material.h"
#include "Mesh.h"
#include "GraphicsNode.h"


namespace Graphics3D {
	class Tetrahedron : public GraphicsNode {
		public:

            Tetrahedron(float size, std::shared_ptr<Material> material,  glm::vec3 position);
            Tetrahedron(float size, std::shared_ptr<Material> material,  glm::vec3 position, 
                    glm::vec3 scale, GLfloat pitch, GLfloat roll, GLfloat yawn);
            ~Tetrahedron();

            glm::vec3 crossProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c);

        private:

            std::shared_ptr<Mesh> genMesh(float size);
        
    };
} 
