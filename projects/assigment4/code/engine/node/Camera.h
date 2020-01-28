#pragma once

#include <glm/glm.hpp>

#include "core/app.h"
#include "render/window.h"
#include "Node.h"
#include "../io/IWASD.h"
#include "../io/IMouse.h"


namespace Graphics3D {
	class Camera : public Node, public IWASD, public IMouse {
		public:

            Camera(Display::Window* window, glm::vec3 cameraPos, glm::vec3 cameraDirection);
            ~Camera();

            void update(float deltaTime) override {}

            glm::mat4 getCameraMatrix();
            glm::vec3 getCameraPos();


            void updateMove(float deltaTime) override;

            void updateLook(float deltaTime) override;

            void setWPressed(bool isWPressed) override;
            void setAPressed(bool isAPressed) override;
            void setSPressed(bool isSPressed) override;
            void setDPressed(bool isDPressed) override;
            void toggleFly() override;

            void setMousePos(float64 xPos, float64 yPos) override;

            glm::vec3 getCameraDirection();

        private:

            Display::Window* window;

            glm::vec3 cameraPos;
            glm::vec3 cameraDirection;

            glm::vec3 cameraUp = glm::vec3(0,1,0);

            bool isWPressed;
            bool isAPressed;
            bool isSPressed;
            bool isDPressed;

            bool isFlyOn;

            float64 mouseXPos;
            float64 mouseYPos;

            float64 mouseLastX;
            float64 mouseLastY;
            float yaw;
            float pitch;

            bool firstMouse;


            void updateM();

    };
} 
