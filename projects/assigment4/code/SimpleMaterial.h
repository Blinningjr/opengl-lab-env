#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp> 

#include "Material.h"


namespace Graphics3D {
	class SimpleMaterial : public Material {
		public:

            SimpleMaterial(std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 color);
            ~SimpleMaterial();

            void applyMaterial() override;

        private:

            glm::vec3 color;

    };
} 
