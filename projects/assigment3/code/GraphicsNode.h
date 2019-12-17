#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>

#include "Material.h"
#include "Mesh.h"



namespace Simple3DGraphics {
	class GraphicsNode {
		public:

            GraphicsNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::mat4 transform);
            ~GraphicsNode();

            void draw();

        private:

            glm::mat4 transform;
            std::shared_ptr<Material> material;
            std::shared_ptr<Mesh> mesh;
        
    };
} 
