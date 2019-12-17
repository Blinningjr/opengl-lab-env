#include "ShaderProgram.h"


namespace Simple3DGraphics {
//  Class: ​ShaderProgram
//      ● Implement a class ShaderProgram that takes multiple shader resources, and links them into a GL Shader program handle.
//      ● Implement a method within your class that activates the GL shader program (via glUseProgram).
//      ● ShaderProgram objects should also be sharable between renderable objects.


    ShaderProgram::ShaderProgram(std::vector<std::shared_ptr<Shader>> shaders) {
        this->shaders = shaders;

        // create a program object
		this->program = glCreateProgram();
        for (int i = 0; i < this->shaders.size(); i++) {
            glAttachShader(this->program, this->shaders[i]->getShader());
        }
		glLinkProgram(this->program);
        
        GLint shaderLogSize;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}
    }


    ShaderProgram::~ShaderProgram() {

    }


    /**
     *  Activate this program, so its shaders are used.
    */
    void ShaderProgram::use() {
        glUseProgram(this->program);
    }


    /**
     *  Gets the Id of uniform with identifier name.
    */
    GLint ShaderProgram::getUniformId(GLchar *name) {
        return glGetUniformLocation(this->program, name);
    }

}
