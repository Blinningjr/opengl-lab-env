#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "../engine/Material.h"
#include "../engine/Mesh.h"
#include "../engine/GraphicsNode.h"


namespace Graphics3D {
	class Tetrahedron : public GraphicsNode {
		public:

            
            Tetrahedron(float size, std::shared_ptr<Material> material,  glm::vec3 position);
            Tetrahedron(float size, std::shared_ptr<Material> material,  glm::vec3 position, float pitchSpeed,
                float rollSpeed, float yawnSpeed);
            Tetrahedron(float size, std::shared_ptr<Material> material,  glm::vec3 position, GLfloat pitch,
                GLfloat roll, GLfloat yawn, float pitchSpeed, float rollSpeed, float yawnSpeed);
            ~Tetrahedron();

            void update(float deltaTime) override;

            glm::vec3 crossProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c);

        private:

            float pitchSpeed;
            float rollSpeed;
            float yawnSpeed;

            std::shared_ptr<Mesh> genMesh(float size);
        
    };
} 
