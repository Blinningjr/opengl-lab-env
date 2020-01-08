#include "LightSource.h"

#include <glm/gtc/matrix_transform.hpp> 


namespace Graphics3D {


    LightSource::LightSource(glm::vec3 lightSourcePos, GLfloat color[3], GLfloat intensity) {
        this->lightSourcePos = lightSourcePos;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->intensity = intensity;
    }


    LightSource::~LightSource() {
        
    }

}
