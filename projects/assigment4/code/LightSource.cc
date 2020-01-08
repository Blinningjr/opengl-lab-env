#include "LightSource.h"

#include <glm/gtc/matrix_transform.hpp> 


namespace Graphics3D {


    LightSource::LightSource(glm::vec3 lightSourcePos, GLfloat color[3], GLfloat intensity) {
        this->lightSourcePos = lightSourcePos;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
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


    GLfloat* LightSource::getColor() {
        return this->color;
    }

    void LightSource::updateM() {
        this->M = glm::mat4(1);
        this->M = glm::translate(this->M, this->lightSourcePos);
    }
}
