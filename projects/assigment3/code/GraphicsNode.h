#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "Material.h"
#include "Mesh.h"


namespace Simple3DGraphics {
	class GraphicsNode {
		public:

            void Draw();

        private:

            glm::mat4 transform;
            std::unique_ptr<Material> material;
            std::unique_ptr<Mesh> mesh;
        
    };
} 
