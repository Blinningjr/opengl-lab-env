#pragma once

#include <glm/glm.hpp>

#include "core/app.h"
#include "render/window.h"
#include "Node.h"


namespace Graphics3D {
	class Camera : public Node {
		public:

            Camera(Display::Window* window, glm::vec3 cameraPos, glm::vec3 cameraDirection);
            ~Camera();

            void update(float deltaTime) override {}

            glm::mat4 getCameraMatrix();
            glm::vec3 getCameraPos();

            void setDeltaTime(float deltaTime);

        private:

            Display::Window* window;

            glm::vec3 cameraPos;
            glm::vec3 cameraDirection;

            glm::vec3 cameraUp = glm::vec3(0,1,0);

            float deltaTime;

            float lastX;
            float lastY;
            float yaw;
            float pitch;

            bool firstMouse;

            void updateM();

    };
} 
