#pragma once

#include <GL/glew.h>

#include "Material.h"


namespace Graphics3D {
	class SimpleMaterial : public Material {
		public:

            SimpleMaterial(std::shared_ptr<ShaderProgram> shaderProgram, GLfloat color[3]);
            ~SimpleMaterial();

            void applyMaterial() override;

        private:

            GLfloat color[3];

    };
} 
