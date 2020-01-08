#include "GraphicsNode.h"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


namespace Graphics3D {
//  Class: ​GraphicsNode
//      ● Implement a graphics node class that holds a mesh resource, material and an instance specific transform.
//      ● You should also implement a "draw" method that binds the mesh's vertex arrayobject, calls the ApplyMaterial 
//          method of the material, and then executes a draw call.


    GraphicsNode::GraphicsNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 scale, GLfloat pitch, GLfloat roll, GLfloat yawn) {
        this->mesh = mesh;
        this->material = material;
        this->position = position;
        this->scale = scale;
        this->pitch = pitch;
        this->roll = roll;
        this->yawn = yawn;
    }


    GraphicsNode::~GraphicsNode() {
        
    }


    /**
     *  Draws the graphical node on screen.
    */
    void GraphicsNode::draw() {
        this->mesh->setupBuffers();
        this->material->applyMaterial();

        GLint objTransformID = this->material->getUniformId("object_transform");
        glUniformMatrix4fv(objTransformID, 1, GL_FALSE, glm::value_ptr(this->M));
        
        glDrawElements(GL_TRIANGLES, mesh->getIndicesSize(),  GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}
