#include "Shader.h"
#include <cstring>
#include <cstdio>


namespace Simple3DGraphics {
//  Class: ​Shader
//      ● Implement a shader class that takes a vertex shader or fragment shader source, and compiles it into a GL Shader handle.
//      ● The handle should be retrievable by some getter method or const public variable.
//      ● You should be able to specify what type of shader the object holds (vertex or fragment), and also check what type it 
//          is (preferably as an enum)

    Shader::Shader(GLchar* shader, ShaderType shaderType) {
        this->shaderType = shaderType;

        // setup shader
        if (shaderType == vertexShader) {
            this->shaderHandle = glCreateShader(GL_VERTEX_SHADER);
        } else if (shaderType == fragmentShader) {
            this->shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
        } else {
            this->shaderHandle  = NULL;
        }
        
		GLint length = (GLint) std::strlen(shader);
		glShaderSource(this->shaderHandle, 1, &shader, &length);
		glCompileShader(this->shaderHandle);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->shaderHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->shaderHandle, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}
    }


    Shader::~Shader() {

    }


    /**
     *  Gets the shader handle.
    */
    GLuint Shader::getShader() {
        return this->shaderHandle;
    }


    /**
     *  Gets the shader type.
    */
    ShaderType Shader::getShaderType() {
        return this->shaderType;
    }

}
