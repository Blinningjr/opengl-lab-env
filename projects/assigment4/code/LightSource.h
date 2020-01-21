#pragma once

#include <glm/glm.hpp>

#include "Node.h"


namespace Graphics3D {
	class LightSource : public Node {
		public:

            LightSource(glm::vec3 lightSourcePos, glm::vec3 color, GLfloat intensity);
            ~LightSource();

            void update() override {}

            glm::mat4 getLightSourceMatrix();
            glm::vec3 getLightSourcePos();
            GLfloat getIntensity();
            glm::vec3 getColor();

        private:

            glm::vec3 lightSourcePos;

            GLfloat intensity;
            glm::vec3 color;

            void updateM();

    };
} 
