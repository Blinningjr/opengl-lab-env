#include <memory>

#include "ShaderProgram.h"


namespace Simple3DGraphics {
	class Material {
		public:

            virtual void ApplyMaterial() = 0;

        private:

            std::unique_ptr<ShaderProgram> shaderProgram;

    };
} 
