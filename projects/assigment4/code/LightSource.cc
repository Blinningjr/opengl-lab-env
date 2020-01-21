#include "LightSource.h"

#include <glm/gtc/matrix_transform.hpp> 


namespace Graphics3D {


    LightSource::LightSource(glm::vec3 lightSourcePos, glm::vec3 color, GLfloat intensity) {
        this->lightSourcePos = lightSourcePos;
        this->color = color;
        this->intensity = intensity;

        this->updateM();
    }


    LightSource::~LightSource() {
        
    }


    glm::mat4 LightSource::getLightSourceMatrix() {
        return this->M;
    }


    glm::vec3 LightSource::getLightSourcePos() {
        return this->lightSourcePos;
    }


    GLfloat LightSource::getIntensity() {
        return this->intensity;
    }


    glm::vec3 LightSource::getColor() {
        return this->color;
    }

    void LightSource::updateM() {
        this->M = glm::mat4(1);
        this->M = glm::translate(this->M, this->lightSourcePos);
    }
}
