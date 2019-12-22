#pragma once

#include <glm/glm.hpp>


namespace Simple3DGraphics {
	class Camera {
		public:

            Camera(glm::vec3 cameraPos, glm::vec3 cameraDirection);
            ~Camera();

            glm::mat4 getCameraMatrix();

        private:

            glm::vec3 cameraPos;
            glm::vec3 cameraDirection;

    };
} 
