#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "../engine/Material.h"
#include "../engine/Mesh.h"
#include "../engine/GraphicsNode.h"


namespace Graphics3D {
	class Cube : public GraphicsNode {
		public:

            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position);
            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position, 
                float pitchSpeed, float rollSpeed, float yawnSpeed);
            Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position, GLfloat pitch, GLfloat roll,
                    GLfloat yawn, float pitchSpeed, float rollSpeed, float yawnSpeed);
            ~Cube();

            void update(float deltaTime) override;

        private:

            float pitchSpeed;
            float rollSpeed;
            float yawnSpeed;

            std::shared_ptr<Mesh> genMesh(glm::vec3 size);
        
    };
} 
