#pragma once

#include <GL/glew.h>
#include <vector>

#include "Vertex.h"


namespace Graphics3D {
	class Mesh {
		public:

            Mesh(std::vector<Vertex> vertices);
            ~Mesh();

            void setupBuffers();

            unsigned int getSize();

        private:

            GLuint vao; // Vertex array object
            GLuint vbo; // Vertex buffer object

            std::vector<Vertex> vertices;

    };
} 
