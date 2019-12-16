#include <GL/glew.h>
#include <vector>
#include "General.h"


namespace Simple3DGraphics {
	class Mesh {
		public:

            Mesh();
            ~Mesh();

            void setupBuffers(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        private:

            GLuint vao; // Vertex array object
            GLuint vbo; // Vertex buffer object
            GLuint ebo; // Element buffer object

    };
} 
