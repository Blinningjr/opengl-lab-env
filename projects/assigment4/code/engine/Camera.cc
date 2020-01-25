#include "config.h"
#include "Camera.h"


#include <glm/gtc/matrix_transform.hpp> 


namespace Graphics3D {


    Camera::Camera(Display::Window* window, glm::vec3 cameraPos, glm::vec3 cameraDirection) {
        this->cameraPos = cameraPos;
        this->cameraDirection = cameraDirection;
        this->window = window;
        this->deltaTime = 0;

        this->lastX = 512;
        this->lastY = 384;

        this->yaw = 0;
        this->pitch = 0;
        this->firstMouse = true;

        bool isWPressed = false;
        bool isAPressed = false;
        bool isSPressed = false;
        bool isDPressed = false;

        // window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
        //     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        //         this->window->Close();
        //     }
        //     float cameraSpeed = 50.0f * deltaTime;
        //     if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        //         this->cameraPos += cameraSpeed * this->cameraDirection;
        //     if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        //         this->cameraPos -= cameraSpeed * this->cameraDirection;
        //     if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        //         this->cameraPos -= glm::normalize(glm::cross(this->cameraDirection, this->cameraUp)) * cameraSpeed;
        //     if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        //         this->cameraPos += glm::normalize(glm::cross(this->cameraDirection, this->cameraUp)) * cameraSpeed;
        // });

        window->SetMouseMoveFunction([this](float64 xPos, float64 yPos) {
            if(this->firstMouse) {
                this->lastX = xPos;
                this->lastY = yPos;
                this->firstMouse = false;
            }
            float xoffset = xPos - lastX;
            float yoffset = lastY - yPos;
            lastX = xPos;
            lastY = yPos;

            const float sensitivity = 0.25f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            this->yaw   += xoffset;
            this->pitch += yoffset;

            if(this->pitch  > 89.0f)
                this->pitch  =  89.0f;
            if(this->pitch  < -89.0f) 
                this->pitch  = -89.0f;
            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            this->cameraDirection = glm::normalize(direction);
        });
    }


    Camera::~Camera() {
        delete[] this->window;
    }


    glm::mat4 Camera::getCameraMatrix() {
        this->updateM();
        return this->M;;
    }


    glm::vec3 Camera::getCameraPos() {
        return this->cameraPos;
    }

    void Camera::setDeltaTime(float deltaTime) {
        this->deltaTime = deltaTime;
    }


    void Camera::updateMove(float deltaTime) {
        float cameraSpeed = 50.0f * deltaTime;
            if (this->isWPressed)
                this->cameraPos += cameraSpeed * this->cameraDirection;

            if (this->isSPressed)
                this->cameraPos -= cameraSpeed * this->cameraDirection;

            if (this->isAPressed)
                this->cameraPos -= glm::normalize(glm::cross(this->cameraDirection, this->cameraUp)) * cameraSpeed;

            if (this->isDPressed)
                this->cameraPos += glm::normalize(glm::cross(this->cameraDirection, this->cameraUp)) * cameraSpeed;
    }

    void Camera::setWPressed(bool isWPressed) {
        this->isWPressed = isWPressed;
    }
    void Camera::setAPressed(bool isAPressed) {
        this->isAPressed = isAPressed;
    }
    void Camera::setSPressed(bool isSPressed) {
        this->isSPressed = isSPressed;
    }
    void Camera::setDPressed(bool isDPressed) {
        this->isDPressed = isDPressed;
    }


    void Camera::updateM() { 
        this->M = glm::lookAt(this->cameraPos, 
            this->cameraPos + this->cameraDirection, this->cameraUp);
    }   

}
