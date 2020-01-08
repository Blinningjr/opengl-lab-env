#pragma once

#include <glm/glm.hpp>

#include "Node.h"


namespace Graphics3D {
	class LightSource : public Node {
		public:

            LightSource(glm::vec3 lightSourcePos, GLfloat color[3], GLfloat intensity);
            ~LightSource();

            void update() override {}

            glm::mat4 getLightSourceMatrix();
            glm::vec3 getLightSourcePos();
            GLfloat getIntensity();
            GLfloat* getColor();

        private:

            glm::vec3 lightSourcePos;

            GLfloat intensity;
            GLfloat color[3];

            void updateM();

    };
} 
