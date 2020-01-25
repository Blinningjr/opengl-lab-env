#include "GraphicsNode.h"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


namespace Graphics3D {
//  Class: ​GraphicsNode
//      ● Implement a graphics node class that holds a mesh resource, material and an instance specific transform.
//      ● You should also implement a "draw" method that binds the mesh's vertex arrayobject, calls the ApplyMaterial 
//          method of the material, and then executes a draw call.


    GraphicsNode::GraphicsNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::vec3 position, 
            glm::vec3 scale, GLfloat pitch, GLfloat roll, GLfloat yawn) {
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
        this->draw(glm::mat4(1));
    }


    /**
     * Draws the graphical node on screen.
     * Uses the "transformMatrix" as the starting matrix for transforming.
     */
    void GraphicsNode::draw(glm::mat4x4 transformMatrix) {
        this->mesh->setupBuffers();
        this->material->applyMaterial();

        this->M = transformMatrix;
        this->M = glm::translate(this->M, this->position);
        this->M = glm::rotate(this->M, this->pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        this->M = glm::rotate(this->M, this->roll, glm::vec3(0.0f, 0.0f, 1.0f));
        this->M = glm::rotate(this->M, this->yawn, glm::vec3(0.0f, 1.0f, 0.0f));
        this->M = glm::scale(this->M, this->scale);

        GLint objTransformID = this->material->getUniformId("object_transform");
        glUniformMatrix4fv(objTransformID, 1, GL_FALSE, glm::value_ptr(this->M));
        
        // glDrawElements(GL_TRIANGLES, mesh->getIndicesSize(),  GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, this->mesh->getSize());

        glBindVertexArray(0);
    }


    glm::vec3 GraphicsNode::crossProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        glm::vec3 v = b - a;
        glm::vec3 u = c - a;
        glm::vec3 normal = {v.y * u.z - v.z * u.y,
                            -(v.x * u.z - v.z * u.x),
                            v.x * u.y - v.y * u.x};
        return normal;
    }


    glm::mat4 GraphicsNode::getM() {
        return this->M;
    }


    glm::vec3 GraphicsNode::getPosition() {
        return this->position;
    }


    glm::vec3 GraphicsNode::getScale() {
        return this->scale;
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


    void GraphicsNode::setScale(glm::vec3 scale) {
        this->scale = scale;
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