#include "GraphicsNode.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <iostream>


namespace Simple3DGraphics {
//  Class: ​GraphicsNode
//      ● Implement a graphics node class that holds a mesh resource, material and an instance specific transform.
//      ● You should also implement a "draw" method that binds the mesh's vertex arrayobject, calls the ApplyMaterial 
//          method of the material, and then executes a draw call.


    GraphicsNode::GraphicsNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::vec3 position, GLfloat pitch, GLfloat roll, GLfloat yawn) {
        this->mesh = mesh;
        this->material = material;
        this->position = position;
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

        this->transform = glm::mat4(1);
        this->transform = glm::translate(this->transform, this->position);
        this->transform = glm::rotate(this->transform, this->pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        this->transform = glm::rotate(this->transform, this->roll, glm::vec3(0.0f, 0.0f, 1.0f));
        this->transform = glm::rotate(this->transform, this->yawn, glm::vec3(0.0f, 1.0f, 0.0f));

        GLint objTransformID = this->material->getUniformId("object_transform");
        glUniformMatrix4fv(objTransformID, 1, GL_FALSE, glm::value_ptr(this->transform));
        
        glDrawElements(GL_TRIANGLES, mesh->getIndicesSize(),  GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }


    glm::vec3 GraphicsNode::getPosition() {
        return this->position;
    }


    GLfloat GraphicsNode::getPitch() {
        return this->pitch;
    }


    GLfloat GraphicsNode::getRoll() {
        return this->roll;
    }


    GLfloat GraphicsNode::getYawn() {
        return this->yawn;
    }
    

    void GraphicsNode::setPosition(glm::vec3 position) {
        this->position = position;
    }


    void GraphicsNode::setPitch(GLfloat pitch) {
        this->pitch = pitch;
    }


    void GraphicsNode::setRoll(GLfloat roll) {
        this->roll = roll;
    }


    void GraphicsNode::setYawn(GLfloat yawn) {
        this->yawn = yawn;
    }

}
