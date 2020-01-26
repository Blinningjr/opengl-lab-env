#include "QuadTreeNode.h"
#include "QuadTreeLeaf.h"


namespace Graphics3D {


    QuadTreeNode::QuadTreeNode(glm::vec2 center, float size) {
        this->center = center;
        this->size = size;
    }


    QuadTreeNode::QuadTreeNode(glm::vec2 center, float size, GraphicsNode* gNode) {
        this->center = center;
        this->size = size;
        this->insertGraphicsNode(gNode);
    }


    QuadTreeNode::~QuadTreeNode() {
        
    }


    void QuadTreeNode::drawNodes(glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                glm::vec2 pFarLeft, glm::vec2 pFarRight, uint frame) {

        float halfSize = size/2;

        if (this->topLeft != nullptr) {
            std::vector<glm::vec3> points;
            points.push_back(glm::vec3(this->center.x - halfSize, 0, this->center.y -halfSize));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y - halfSize));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x - halfSize, 0, this->center.y));
            if (QuadTree::wholeShapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                this->topLeft->drawAll(frame);
            } else if (QuadTree::shapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                this->topLeft->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
            }
        }

        if (this->topRight != nullptr) {
            std::vector<glm::vec3> points;
            points.push_back(glm::vec3(this->center.x, 0, this->center.y - halfSize));
            points.push_back(glm::vec3(this->center.x + halfSize, 0, this->center.y - halfSize));
            points.push_back(glm::vec3(this->center.x + halfSize, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y));
            if (QuadTree::wholeShapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                this->topRight->drawAll(frame);
            } else if (QuadTree::shapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                this->topRight->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
            }
        }

        if (this->bottomLeft != nullptr) {
            std::vector<glm::vec3> points;
            points.push_back(glm::vec3(this->center.x - halfSize, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y + halfSize));
            points.push_back(glm::vec3(this->center.x - halfSize, 0, this->center.y + halfSize));
            if (QuadTree::wholeShapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                this->bottomLeft->drawAll(frame);
            } else if (QuadTree::shapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                this->bottomLeft->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
            }
        }

        if (this->bottomRight != nullptr) {
            std::vector<glm::vec3> points;
            points.push_back(glm::vec3(this->center.x, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x + halfSize, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x + halfSize, 0, this->center.y + halfSize));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y + halfSize));
            if (QuadTree::wholeShapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                this->bottomRight->drawAll(frame);
            } else if (QuadTree::shapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                this->bottomRight->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
            }
        }
        
    }


    void QuadTreeNode::drawAll(uint frame) {
        if (this->topLeft != nullptr)
            this->topLeft->drawAll(frame);

        if (this->topRight != nullptr)
            this->topRight->drawAll(frame);

        if (this->bottomLeft != nullptr)
            this->bottomLeft->drawAll(frame);

        if (this->bottomRight != nullptr)
            this->bottomRight->drawAll(frame);
    }
       

    bool QuadTreeNode::isLeaf() {
        return false;
    }


    void QuadTreeNode::insertGraphicsNode(GraphicsNode* gNode) {
        
        bool* insideQuads = this->checkInsideWhichQuads(gNode);
        float childeSize = this->size/2.0f;
        
        if (insideQuads[0]) {
            if (this->topLeft != nullptr) {
                if (this->topLeft->isLeaf()) {
                    GraphicsNode* oldGNode = ((QuadTreeLeaf*)this->topLeft)->getGraphicsNode();
                    this->topLeft = new QuadTreeNode(this->calcTopLeftCenter(), childeSize);
                    ((QuadTreeNode*)this->topLeft)->insertGraphicsNode(oldGNode);
                }
                ((QuadTreeNode*)this->topLeft)->insertGraphicsNode(gNode);   
            } else {
                this->topLeft = new QuadTreeLeaf(gNode);
            }
        }

        if (insideQuads[1]) {
            if (this->topRight != nullptr) {
                if (this->topRight->isLeaf()) {
                    GraphicsNode* oldGNode = ((QuadTreeLeaf*)this->topRight)->getGraphicsNode();
                    this->topRight = new QuadTreeNode(this->calcTopRightCenter(), childeSize);
                    ((QuadTreeNode*)this->topRight)->insertGraphicsNode(oldGNode);
                }
                ((QuadTreeNode*)this->topRight)->insertGraphicsNode(gNode);   
            } else {
                this->topRight = new QuadTreeLeaf(gNode);
            }
        }

        if (insideQuads[2]) {
            if (this->bottomLeft != nullptr) {
                if (this->bottomLeft->isLeaf()) {
                    GraphicsNode* oldGNode = ((QuadTreeLeaf*)this->bottomLeft)->getGraphicsNode();
                    this->bottomLeft = new QuadTreeNode(this->calcBottomLeftCenter(), childeSize);
                    ((QuadTreeNode*)this->bottomLeft)->insertGraphicsNode(oldGNode);
                }
                ((QuadTreeNode*)this->bottomLeft)->insertGraphicsNode(gNode);   
            } else {
                this->bottomLeft = new QuadTreeLeaf(gNode);
            }
        }

        if (insideQuads[3]) {
            if (this->bottomRight != nullptr) {
                if (this->bottomRight->isLeaf()) {
                    GraphicsNode* oldGNode = ((QuadTreeLeaf*)this->bottomRight)->getGraphicsNode();
                    this->bottomRight = new QuadTreeNode(this->calcBottomRightCenter(), childeSize);
                    ((QuadTreeNode*)this->bottomRight)->insertGraphicsNode(oldGNode);
                }
                ((QuadTreeNode*)this->bottomRight)->insertGraphicsNode(gNode);   
            } else {
                this->bottomRight = new QuadTreeLeaf(gNode);
            }
        }
        
    }


    bool* QuadTreeNode::checkInsideWhichQuads(GraphicsNode* gNode) {
        glm::vec3 nodePosition = gNode->getPosition();
        glm::vec2 position(nodePosition.x, nodePosition.z);
        bool insideQuads[] = {false, false, false, false};

        if (position.x < this->center.x) {
            // left
            if (position.y < this->center.y) {
                // top
                insideQuads[0] = true;
            } else if (position.y < this->center.y) {
                // bottom
                insideQuads[2] = true;
            } else {
                // top and bottom
                insideQuads[0] = true;
                insideQuads[2] = true;
            }

        } else if (position.x > this->center.x) {
            // right
            if (position.y < this->center.y) {
                // top
                insideQuads[1] = true;
            } else if (position.y < this->center.y) {
                // bottom
                insideQuads[3] = true;
            } else {
                // top and bottom
                insideQuads[1] = true;
                insideQuads[3] = true;
            }
        } else {
            // left and right
            if (position.y < this->center.y) {
                // top
                insideQuads[0] = true;
                insideQuads[1] = true;
            } else if (position.y < this->center.y) {
                // bottom
                insideQuads[2] = true;
                insideQuads[3] = true;
            } else {
                // top and bottom
                insideQuads[0] = true;
                insideQuads[1] = true;
                insideQuads[2] = true;
                insideQuads[3] = true;
            }
        }
        return insideQuads;
    }

    glm::vec2 QuadTreeNode::calcTopLeftCenter() {
        return glm::vec2(this->center.x - this->size/2.0f, this->center.y - this->size/2.0f);
    }

    glm::vec2 QuadTreeNode::calcTopRightCenter() {
        return glm::vec2(this->center.x + this->size/2.0f, this->center.y - this->size/2.0f);
    }

    glm::vec2 QuadTreeNode::calcBottomLeftCenter() {
        return glm::vec2(this->center.x - this->size/2.0f, this->center.y + this->size/2.0f);
    }

    glm::vec2 QuadTreeNode::calcBottomRightCenter() {
        return glm::vec2(this->center.x + this->size/2.0f, this->center.y + this->size/2.0f);
    }
}
