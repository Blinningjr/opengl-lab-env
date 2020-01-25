#include "config.h"
#include "Camera.h"


#include <glm/gtc/matrix_transform.hpp> 


namespace Graphics3D {


    Camera::Camera(Display::Window* window, glm::vec3 cameraPos, glm::vec3 cameraDirection) {
        this->cameraPos = cameraPos;
        this->cameraDirection = cameraDirection;
        this->window = window;

        this->isWPressed = false;
        this->isAPressed = false;
        this->isSPressed = false;
        this->isDPressed = false;
        
        this->isFlyOn = true;

        this->mouseXPos = 512;
        this->mouseYPos = 384;

        this->mouseLastX = 512;
        this->mouseLastY = 384;

        this->yaw = 0;
        this->pitch = 0;
        this->firstMouse = true;
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


    void Camera::updateMove(float deltaTime) {
        float cameraSpeed = 25.0f * deltaTime;
        if (this->isWPressed)
            this->cameraPos += cameraSpeed * this->cameraDirection;

        if (this->isSPressed)
            this->cameraPos -= cameraSpeed * this->cameraDirection;

        if (this->isAPressed)
            this->cameraPos -= glm::normalize(glm::cross(this->cameraDirection, this->cameraUp)) * cameraSpeed/2.0f;

        if (this->isDPressed)
            this->cameraPos += glm::normalize(glm::cross(this->cameraDirection, this->cameraUp)) * cameraSpeed/2.0f;

        if (this->isFlyOn)
            this->cameraPos.y = 2.5;
    }


    void Camera::updateLook(float deltaTime) {
        
        float64 xoffset = this->mouseXPos - this->mouseLastX;
        float64 yoffset = this->mouseLastY - this->mouseYPos;
        this->mouseLastX = this->mouseXPos;
        this->mouseLastY = this->mouseYPos;

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

    void Camera::toggleFly() {
        this->isFlyOn = !this->isFlyOn;
    }


    void Camera::setMousePos(float64 xPos, float64 yPos) {
        if (this->firstMouse) {
            this->firstMouse = false;
            this->mouseLastX = xPos;
            this->mouseLastY = yPos;
        }
        this->mouseXPos = xPos;
        this->mouseYPos = yPos;
    }


    void Camera::updateM() { 
        this->M = glm::lookAt(this->cameraPos, 
            this->cameraPos + this->cameraDirection, this->cameraUp);
    }   

}
