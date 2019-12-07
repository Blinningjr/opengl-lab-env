#include <GL/glew.h>

#include "Shader.h"


namespace Simple3DGraphics {
	class ShaderProgram {
		public:
        private:

            std::unique_ptr<std::shared_ptr<Shader>[]> shaders;
            GLuint program;

    };
} 
