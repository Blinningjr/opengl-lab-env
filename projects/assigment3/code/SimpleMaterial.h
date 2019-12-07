#include <GL/glew.h>

#include "Material.h"


namespace Simple3DGraphics {
	class SimpleMaterial : Material {
		public:

            void ApplyMaterial() override;

        private:

            GLfloat color[3];

    };
} 
