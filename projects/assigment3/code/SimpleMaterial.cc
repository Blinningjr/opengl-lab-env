#include "SimpleMaterial.h"


namespace Simple3DGraphics {
//  Class: SimpleMaterial
//      ● Implement a​​ subclass to Material, called SimpleMaterial, that holds a RGB color variable.
//      ● The ApplyMaterial method should activate the program of the material and send the color to the program as a uniform.
//      ● The material should automatically (upon construction) load shaders that do the following:
//          ○ Transform the vertices of a mesh with the GraphicsNodes object transform(objectTransform * vertexPostion).
//          ○ Color the pixels according to the uniform color set by the material.


    SimpleMaterial::SimpleMaterial(std::shared_ptr<ShaderProgram> shaderProgram, GLfloat color[3]) {
        this->shaderProgram = shaderProgram;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
    }


    SimpleMaterial::~SimpleMaterial() {
        
    }


    /**
     *  Applays shaders and color. 
    */
    void SimpleMaterial::applyMaterial() {
        this->shaderProgram->use();
        GLint colorID = this->shaderProgram->getUniformId("color");
        glUniform3f(colorID, this->color[0], this->color[1], this->color[2]);
    }

}
