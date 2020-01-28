#pragma once

#include <memory>
#include <GL/glew.h>
#include <vector>

#include "Shader.h"


namespace Graphics3D {
	class ShaderProgram {
		public:

            ShaderProgram(std::vector<std::shared_ptr<Shader>> shaders);
            ~ShaderProgram();

            void use();

            GLint getUniformId(GLchar *name);

        private:

            std::vector<std::shared_ptr<Shader>> shaders;
            GLuint program;

    };
} 
