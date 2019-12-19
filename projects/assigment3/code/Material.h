#pragma once


#include <memory>

#include "ShaderProgram.h"


namespace Simple3DGraphics {
	class Material {
		public:

            virtual void applyMaterial() = 0;

            GLint getUniformId(GLchar *name) {
                return shaderProgram->getUniformId(name);
            }

        protected:

            std::shared_ptr<ShaderProgram> shaderProgram;

    };
} 
