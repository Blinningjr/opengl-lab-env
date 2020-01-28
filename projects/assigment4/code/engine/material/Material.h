#pragma once


#include <memory>

#include "../shader/ShaderProgram.h"


namespace Graphics3D {
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
