#include "QuadTreeLeaf.h"
#include "../utils/Vertex.h"


namespace Graphics3D {


    QuadTreeLeaf::QuadTreeLeaf(GraphicsNode* gNode) {
        this->gNode = gNode;
    }


    QuadTreeLeaf::~QuadTreeLeaf() {
    }


    /**
     *  Draws the object in this leaf if it is visable by the view.
    */
    void QuadTreeLeaf::drawNodes(glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                glm::vec2 pFarLeft, glm::vec2 pFarRight, uint frame) {
        if (this->gNode->getFrame() != frame) {
            glm::vec3 pos = this->gNode->getPosition();
            std::vector<Vertex> vertices = this->gNode->getVertices();
            std::vector<glm::vec3> points;
            for (int i = 0; i < vertices.size(); i++) {
                points.push_back(vertices[i].pos + pos);
            }
            if (QuadTree::shapePartlyInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight) > 0) {
                this->gNode->draw();
                this->gNode->setFrame(frame);
            }
        }
    }


    /**
     * Draws the object in this leaf.
    */
    void QuadTreeLeaf::drawAll(uint frame) {
        if (this->gNode->getFrame() != frame) {
            this->gNode->draw();
            this->gNode->setFrame(frame);
        }
    }


    /**
     *  Get the GraphicsNode pointer stored in the leaf.
    */
    GraphicsNode* QuadTreeLeaf::getGraphicsNode() {
        return this->gNode;
    }


    /**
     *  Returns true if this is a leaf.
    */
    bool QuadTreeLeaf::isLeaf() {
        return true;
    }
}
