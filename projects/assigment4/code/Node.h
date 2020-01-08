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

            glm::vec3 getPosition() {
                return this->position;
            }


            glm::vec3 getScale() {
                return this->scale;
            }


            GLfloat getPitch() {
                return this->pitch;
            }


            GLfloat getRoll() {
                return this->roll;
            }


            GLfloat getYawn() {
                return this->yawn;
            }


            void setPosition(glm::vec3 position) {
                this->position = position;
                this->updateM();
            }


            void setScale(glm::vec3 scale) {
                this->scale = scale;
                this->updateM();
            }


            void setPitch(GLfloat pitch) {
                this->pitch = pitch;
                this->updateM();
            }


            void setRoll(GLfloat roll) {
                this->roll = roll;
                this->updateM();
            }


            void setYawn(GLfloat yawn) {
                this->yawn = yawn;
                this->updateM();
            }

        protected:

            glm::vec3 position;
            glm::vec3 scale;
            GLfloat pitch;
            GLfloat roll;
            GLfloat yawn;

            glm::mat4 M; 

            std::vector<std::shared_ptr<Node>> L;


            void updateM() {
                this->M = glm::mat4(1);
                this->M = glm::translate(this->M, this->position);
                this->M = glm::rotate(this->M, this->pitch, glm::vec3(1.0f, 0.0f, 0.0f));
                this->M = glm::rotate(this->M, this->roll, glm::vec3(0.0f, 0.0f, 1.0f));
                this->M = glm::rotate(this->M, this->yawn, glm::vec3(0.0f, 1.0f, 0.0f));
                this->M = glm::scale(this->M, this->scale);
            }

    };
} 
