#include <memory>

#include "ShaderProgram.h"


namespace Simple3DGraphics {
	class Material {
		public:

            virtual void applyMaterial() = 0;

        protected:

            std::shared_ptr<ShaderProgram> shaderProgram;

    };
} 
