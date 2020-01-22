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

            GraphicsNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,  glm::vec3 position,
                glm::vec3 scale, GLfloat pitch, GLfloat roll, GLfloat yawn);
            ~GraphicsNode();

            void draw();

            void draw(glm::mat4x4 transformMatrix);

            void update() override {}

            glm::mat4 getM();
            glm::vec3 getPosition();
            glm::vec3 getScale();
            GLfloat getPitch();
            GLfloat getRoll();
            GLfloat getYawn();

            void setPosition(glm::vec3 position);
            void setScale(glm::vec3 scale);
            void setPitch(GLfloat pitch);
            void setRoll(GLfloat roll);
            void setYawn(GLfloat yawn);

        protected:

            glm::vec3 position;
            glm::vec3 scale;
            GLfloat pitch;
            GLfloat roll;
            GLfloat yawn;

            std::shared_ptr<Material> material;
            std::shared_ptr<Mesh> mesh;
        
    };
} 
