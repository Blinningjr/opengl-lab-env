#include <memory>
#include <GL/glew.h>
#include <vector>

#include "Shader.h"


namespace Simple3DGraphics {
	class ShaderProgram {
		public:

            ShaderProgram(std::vector<std::shared_ptr<Shader>> shaders);
            ~ShaderProgram();

            void activateProgram();

            GLuint getUniformId(GLchar *name);

        private:

            std::vector<std::shared_ptr<Shader>> shaders;
            GLuint program;

    };
} 
