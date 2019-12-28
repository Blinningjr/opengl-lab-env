#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "Material.h"
#include "Mesh.h"



namespace Simple3DGraphics {
	class GraphicsNode {
		public:

            GraphicsNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,  glm::vec3 position, glm::vec3 scale, GLfloat pitch, GLfloat roll, GLfloat yawn);
            ~GraphicsNode();

            void draw();

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

        private:

            glm::vec3 position;
            glm::vec3 scale;
            GLfloat pitch;
            GLfloat roll;
            GLfloat yawn;

            glm::mat4 transform;
            std::shared_ptr<Material> material;
            std::shared_ptr<Mesh> mesh;
        
    };
} 
