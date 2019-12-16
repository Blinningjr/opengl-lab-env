#include <GL/glew.h>

#include "ShaderType.h"


namespace Simple3DGraphics {
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
