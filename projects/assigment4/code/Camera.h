#pragma once

#include <glm/glm.hpp>


namespace Graphics3D {
	class Camera {
		public:

            Camera(glm::vec3 cameraPos, glm::vec3 cameraDirection);
            ~Camera();

            glm::mat4 getCameraMatrix();
            glm::vec3 getCameraPos();

        private:

            glm::vec3 cameraPos;
            glm::vec3 cameraDirection;

    };
} 
