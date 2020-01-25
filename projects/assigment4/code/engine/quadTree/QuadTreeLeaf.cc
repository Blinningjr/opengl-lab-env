#include "QuadTreeLeaf.h"


namespace Graphics3D {


    QuadTreeLeaf::QuadTreeLeaf(GraphicsNode* gNode) {
        this->gNode = gNode;
    }


    QuadTreeLeaf::~QuadTreeLeaf() {
    }

    std::vector<GraphicsNode*> QuadTreeLeaf::getGraphicsNodes() {
        std::vector<GraphicsNode*> gNodes;
        gNodes.push_back(this->gNode);
        return gNodes;
    }


    GraphicsNode* QuadTreeLeaf::getGraphicsNode() {
        return this->getGraphicsNode;
    }


    bool QuadTreeLeaf::isLeaf() {
        return true;
    }
}
