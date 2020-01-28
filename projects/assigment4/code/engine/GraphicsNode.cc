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

        this->frame = 0;
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
        
        glDrawArrays(GL_TRIANGLES, 0, this->mesh->getSize());

        glBindVertexArray(0);
    }


    /**
     * Gets the GraphicsNodes transform matrix.
     * Note: The matrix is only updated on draw calls. 
    */
    glm::mat4 GraphicsNode::getM() {
        return this->M;
    }


    /**
     *  Gets the GraphicsNodes position.
    */
    glm::vec3 GraphicsNode::getPosition() {
        return this->position;
    }


    /**
     *  Gets the GraphicsNodes scale.
    */
    glm::vec3 GraphicsNode::getScale() {
        return this->scale;
    }


    /**
     *  Gets the GraphicsNodes pitch.
    */
    GLfloat GraphicsNode::getPitch() {
        return this->pitch;
    }


    /**
     *  Gets the GraphicsNodes roll.
    */
    GLfloat GraphicsNode::getRoll() {
        return this->roll;
    }


    /**
     *  Gets the GraphicsNodes yawn.
    */
    GLfloat GraphicsNode::getYawn() {
        return this->yawn;
    }


    /**
     *  Sets the GraphicsNodes position.
    */
    void GraphicsNode::setPosition(glm::vec3 position) {
        this->position = position;
    }


    /**
     *  Sets the GraphicsNodes scale.
    */
    void GraphicsNode::setScale(glm::vec3 scale) {
        this->scale = scale;
    }


    /**
     *  Sets the GraphicsNodes pitch.
    */
    void GraphicsNode::setPitch(GLfloat pitch) {
        this->pitch = pitch;
    }


    /**
     *  Sets the GraphicsNodes roll.
    */
    void GraphicsNode::setRoll(GLfloat roll) {
        this->roll = roll;
    }


    /**
     *  Sets the GraphicsNodes yawn.
    */
    void GraphicsNode::setYawn(GLfloat yawn) {
        this->yawn = yawn;
    }


    /**
     *  Get the GraphicsNodes frame.
     *  Frame is the last frame the GraphicsNode whs drawn.
    */
    uint GraphicsNode::getFrame() {
        return this->frame;
    }
    

    /**
     *  Sets the GraphicsNodes frame.
     *  Frame is the last frame the GraphicsNode whs drawn.
    */
    void GraphicsNode::setFrame(uint frame) {
        this->frame = frame;
    }


    /**
     *  Get the GraphicsNodes mesh vertices.
    */
    std::vector<Vertex> GraphicsNode::getVertices() {
        return this->mesh->getVertices();
    }
}
