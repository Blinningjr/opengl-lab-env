#pragma once

#include <GL/glew.h>

#include "../utils/ShaderType.h"


namespace Graphics3D {
	class Shader {
		public:

            Shader(GLchar* shader, ShaderType shaderType);
            ~Shader();

            GLuint getShader();
            ShaderType getShaderType();

        private:

            GLuint shaderHandle;
            ShaderType shaderType;

    };
} 
