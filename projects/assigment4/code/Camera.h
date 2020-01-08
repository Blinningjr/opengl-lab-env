#pragma once

#include <glm/glm.hpp>

#include "Node.h"


namespace Graphics3D {
	class Camera : public Node {
		public:

            Camera(glm::vec3 cameraPos, glm::vec3 cameraDirection);
            ~Camera();

            void update() override {}

            glm::mat4 getCameraMatrix();
            glm::vec3 getCameraPos();

        private:

            glm::vec3 cameraPos;
            glm::vec3 cameraDirection;


            void updateM();

    };
} 
