#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp> 


namespace Graphics3D {


    Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraDirection) {
        this->cameraPos = cameraPos;
        this->cameraDirection = cameraDirection;
        
    }


    Camera::~Camera() {
        
    }


    glm::mat4 Camera::getCameraMatrix() {
        this->updateM();
        return this->M;;
    }


    glm::vec3 Camera::getCameraPos() {
        return this->cameraPos;
    }

    void Camera::updateM() { 
        this->M = glm::lookAt(this->cameraPos, 
            this->cameraPos + this->cameraDirection, glm::vec3(0, 1, 0));
    }   

}
