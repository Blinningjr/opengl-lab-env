#include <GL/glew.h>

#include "Material.h"


namespace Simple3DGraphics {
	class SimpleMaterial : public Material {
		public:

            SimpleMaterial(std::shared_ptr<ShaderProgram> shaderProgram, GLfloat color[3]);
            ~SimpleMaterial();

            void applyMaterial() override;

        private:

            GLfloat color[3];

    };
} 
