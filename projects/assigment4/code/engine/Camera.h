#pragma once

#include <glm/glm.hpp>

#include "core/app.h"
#include "render/window.h"
#include "Node.h"
#include "IWASD.h"


namespace Graphics3D {
	class Camera : public Node, public IWASD {
		public:

            Camera(Display::Window* window, glm::vec3 cameraPos, glm::vec3 cameraDirection);
            ~Camera();

            void update(float deltaTime) override {}

            glm::mat4 getCameraMatrix();
            glm::vec3 getCameraPos();

            void setDeltaTime(float deltaTime);


            void updateMove(float deltaTime) override;

            void setWPressed(bool isWPressed) override;
            void setAPressed(bool isAPressed) override;
            void setSPressed(bool isSPressed) override;
            void setDPressed(bool isDPressed) override;

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

            bool isWPressed;
            bool isAPressed;
            bool isSPressed;
            bool isDPressed;


            void updateM();

    };
} 
