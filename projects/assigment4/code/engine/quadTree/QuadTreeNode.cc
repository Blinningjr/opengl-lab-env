#include "QuadTreeNode.h"
#include "QuadTreeLeaf.h"
#include "iostream"


namespace Graphics3D {


    QuadTreeNode::QuadTreeNode(glm::vec2 center, float size, uint depth) {
        this->center = center;
        this->size = size;

        this->topLeft = NULL;
        this->topRight = NULL;
        this->bottomLeft = NULL;
        this->bottomRight = NULL;
    
        this->depth = depth;
    }


    QuadTreeNode::QuadTreeNode(glm::vec2 center, float size, GraphicsNode* gNode) {
        this->center = center;
        this->size = size;

        this->topLeft = NULL;
        this->topRight = NULL;
        this->bottomLeft = NULL;
        this->bottomRight = NULL;

        this->insertGraphicsNode(gNode);
    }


    QuadTreeNode::~QuadTreeNode() {
        
    }


    void QuadTreeNode::drawNodes(glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                glm::vec2 pFarLeft, glm::vec2 pFarRight, uint frame) {

        float halfSize = size/2;

        if (this->topLeft != NULL) {
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

        if (this->topRight != NULL) {
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

        if (this->bottomLeft != NULL) {
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

        if (this->bottomRight != NULL) {
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
        // std::cout << "\n";
        // std::cout << this->depth;

        if (this->topLeft != NULL) {
            this->topLeft->drawAll(frame);
        }

        if (this->topRight != NULL) {
            this->topRight->drawAll(frame);
        }
            
        if (this->bottomLeft != NULL) {
            this->bottomLeft->drawAll(frame);
        }
            
        if (this->bottomRight != NULL) {
            this->bottomRight->drawAll(frame);
        }
    }
       

    bool QuadTreeNode::isLeaf() {
        return false;
    }


    void QuadTreeNode::insertGraphicsNode(GraphicsNode* gNode) {
        bool iQuads[4] = {false, false, false, false};
        bool* insideQuads = this->checkInsideWhichQuads(iQuads, gNode);
        float childeSize = this->size/2.0f;

        // std::cout << "\n";
        // std::cout << insideQuads[0];
        // std::cout << ":";
        // std::cout << insideQuads[1];
        // std::cout << ":";
        // std::cout << insideQuads[2];
        // std::cout << ":";
        // std::cout << insideQuads[3];
        // std::cout << "\n";
        
        if (insideQuads[0]) {
            if (this->topLeft != NULL) {
                if (this->topLeft->isLeaf()) {
                    GraphicsNode* oldGNode = ((QuadTreeLeaf*)this->topLeft)->getGraphicsNode();
                    delete this->topLeft;
                    this->topLeft = new QuadTreeNode(this->calcTopLeftCenter(), childeSize, this->depth + 1);
                    ((QuadTreeNode*)this->topLeft)->insertGraphicsNode(oldGNode);
                     ((QuadTreeNode*)this->topLeft)->insertGraphicsNode(gNode); 
                } else {
                    ((QuadTreeNode*)this->topLeft)->insertGraphicsNode(gNode);   
                }
            } else {
                this->topLeft = new QuadTreeLeaf(gNode);
            }
        }

        if (insideQuads[1]) {
            if (this->topRight != NULL) {
                if (this->topRight->isLeaf()) {
                    GraphicsNode* oldGNode = ((QuadTreeLeaf*)this->topRight)->getGraphicsNode();
                    delete this->topRight;
                    this->topRight = new QuadTreeNode(this->calcTopRightCenter(), childeSize, this->depth + 1);
                    ((QuadTreeNode*)this->topRight)->insertGraphicsNode(oldGNode);
                    ((QuadTreeNode*)this->topRight)->insertGraphicsNode(gNode);
                } else {
                    ((QuadTreeNode*)this->topRight)->insertGraphicsNode(gNode);
                }
            } else {
                this->topRight = new QuadTreeLeaf(gNode);
            }
        }

        if (insideQuads[2]) {
            if (this->bottomLeft != NULL) {
                if (this->bottomLeft->isLeaf()) {
                    GraphicsNode* oldGNode = ((QuadTreeLeaf*)this->bottomLeft)->getGraphicsNode();
                    delete this->bottomLeft;
                    this->bottomLeft = new QuadTreeNode(this->calcBottomLeftCenter(), childeSize, this->depth + 1);
                    ((QuadTreeNode*)this->bottomLeft)->insertGraphicsNode(oldGNode);
                    ((QuadTreeNode*)this->bottomLeft)->insertGraphicsNode(gNode); 
                } else {
                    ((QuadTreeNode*)this->bottomLeft)->insertGraphicsNode(gNode);   
                }
            } else {
                this->bottomLeft = new QuadTreeLeaf(gNode);
            }
        }

        if (insideQuads[3]) {
            if (this->bottomRight != NULL) {
                if (this->bottomRight->isLeaf()) {
                    GraphicsNode* oldGNode = ((QuadTreeLeaf*)this->bottomRight)->getGraphicsNode();
                    delete this->bottomRight;
                    this->bottomRight = new QuadTreeNode(this->calcBottomRightCenter(), childeSize, this->depth + 1);
                    ((QuadTreeNode*)this->bottomRight)->insertGraphicsNode(oldGNode);
                    ((QuadTreeNode*)this->bottomRight)->insertGraphicsNode(gNode);  
                } else {
                    ((QuadTreeNode*)this->bottomRight)->insertGraphicsNode(gNode);  
                } 
            } else {
                this->bottomRight = new QuadTreeLeaf(gNode);
            }
        }
    }


    bool* QuadTreeNode::checkInsideWhichQuads(bool* insideQuads, GraphicsNode* gNode) {
        glm::vec3 nodePosition = gNode->getPosition();
        glm::vec2 position(nodePosition.x, nodePosition.z);

        if (position.x < this->center.x) {
            // left
            if (position.y < this->center.y) {
                // top
                insideQuads[0] = true;
            } else if (position.y > this->center.y) {
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
            } else if (position.y > this->center.y) {
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
            } else if (position.y > this->center.y) {
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
