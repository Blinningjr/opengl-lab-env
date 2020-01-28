#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "../../material/Material.h"
#include "../../Mesh.h"
#include "../GraphicsNode.h"


namespace Graphics3D {
	class Cube : public GraphicsNode {
		public:

            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position);
            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position, GLfloat yawn);
            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position, 
                float pitchSpeed, float rollSpeed, float yawnSpeed);
            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position, GLfloat pitch, GLfloat roll,
                    GLfloat yawn, float pitchSpeed, float rollSpeed, float yawnSpeed);

            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position, GLfloat pitch, GLfloat roll,
                GLfloat yawn, float moveSpeed, float moveDistance);
        
            ~Cube();

            void update(float deltaTime) override;

            static std::shared_ptr<Mesh> genMesh(glm::vec3 size);

        private:

            float pitchSpeed;
            float rollSpeed;
            float yawnSpeed;

            float moveSpeed;
            float moveDistance;
            float angle;
            float originalY;
        
    };
} 
