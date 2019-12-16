#include "Mesh.h"


namespace Simple3DGraphics {
// Class:: ​Mesh
    // ● Implement a class that acts as a sharable resource for renderable objects.
    // ● The Mesh class should house a Vertex Array Object handle, Vertex Buffer handleand 
    //      Element Array Buffer handle (index buffer) that are all generated via OpenGLcalls.
    // ● The mesh resource class should be able to take an array, list or vector of vertex datafrom 
    //      your CPU and set up the aforementioned buffers on the GPU.
    // ● Remember to setup your vertex attribute pointers accordingly.
    // ● The idea with this (and all other) resource classes is that they should be sharablebetween renderable objects.


    Mesh::Mesh() {
        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);
        glGenBuffers(1, &this->ebo);
    }


    Mesh::~Mesh() {

    }


    /**
     *  Sets up the buffers on the GPU.
    */
    void Mesh::setupBuffers(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);

        glBindVertexArray(0);
    }

}
