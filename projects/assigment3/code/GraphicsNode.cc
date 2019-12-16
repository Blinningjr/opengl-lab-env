#include "GraphicsNode.h"


namespace Simple3DGraphics {
//  Class: ​GraphicsNode
//      ● Implement a graphics node class that holds a mesh resource, material and an instance specific transform.
//      ● You should also implement a "draw" method that binds the mesh's vertex arrayobject, calls the ApplyMaterial 
//          method of the material, and then executes a draw call.


    GraphicsNode::GraphicsNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::mat4 transform) {
        this->mesh = mesh;
        this->material = material;
        this->transform = transform;
    }


    GraphicsNode::~GraphicsNode() {
        
    }


    /**
     *  Draws the graphical node on screen.
    */
    void GraphicsNode::Draw() {
        this->mesh->setupBuffers();
        this->material->applyMaterial();

        glDrawElements(GL_TRIANGLES, mesh->getIndicesSize(),  GL_UNSIGNED_INT, 0);
    }

}
