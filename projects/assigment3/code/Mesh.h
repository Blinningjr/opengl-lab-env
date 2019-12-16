#include <GL/glew.h>
#include <vector>

#include "Vertex.h"


namespace Simple3DGraphics {
	class Mesh {
		public:

            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
            ~Mesh();

            void setupBuffers();

            unsigned int getIndicesSize();

        private:

            GLuint vao; // Vertex array object
            GLuint vbo; // Vertex buffer object
            GLuint ebo; // Element buffer object

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

    };
} 
