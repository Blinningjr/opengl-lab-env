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

        std::vector<glm::vec2> viewPoints;
        viewPoints.push_back(pCloseLeft);
        viewPoints.push_back(pCloseRight);
        viewPoints.push_back(pFarLeft);
        viewPoints.push_back(pFarRight);
        viewPoints.push_back(pFarLeft + (pFarRight - pFarLeft)/2.0f + (pFarRight - pFarLeft)/2.0f);

        float halfSize = this->size/2.0f;

        if (this->topLeft != NULL) {
            std::vector<glm::vec3> points;
            points.push_back(glm::vec3(this->center.x - this->size, 0, this->center.y -this->size));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y - this->size));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x - this->size, 0, this->center.y));
            glm::vec2 pos = this->calcTopLeftCenter();
            points.push_back(glm::vec3(pos.x, 0, pos.y));
            int isInside = QuadTree::shapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight);
            if (isInside == 2) {
                this->topLeft->drawAll(frame);
            } else if (isInside == 1) {
                this->topLeft->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
            } else {
                glm::vec2 childeCenter(this->center.x - halfSize, this->center.y - halfSize);
                if (this->checkViewInside(childeCenter, viewPoints)) {
                    this->topLeft->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
                }
            }
        }

        if (this->topRight != NULL) {
            std::vector<glm::vec3> points;
            points.push_back(glm::vec3(this->center.x, 0, this->center.y - this->size));
            points.push_back(glm::vec3(this->center.x + this->size, 0, this->center.y - this->size));
            points.push_back(glm::vec3(this->center.x + this->size, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y));
            glm::vec2 pos = this->calcTopRightCenter();
            points.push_back(glm::vec3(pos.x, 0, pos.y));
            int isInside = QuadTree::shapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight);
            if (isInside == 2) {
                this->topRight->drawAll(frame);
            } else if (isInside == 1) {
                this->topRight->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
            } else {
                glm::vec2 childeCenter(this->center.x + halfSize, this->center.y - halfSize);
                if (this->checkViewInside(childeCenter, viewPoints)) {
                    this->topRight->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
                }
            }
        }

        if (this->bottomLeft != NULL) {
            std::vector<glm::vec3> points;
            points.push_back(glm::vec3(this->center.x - this->size, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y + this->size));
            points.push_back(glm::vec3(this->center.x - this->size, 0, this->center.y + this->size));
            glm::vec2 pos = this->calcBottomLeftCenter();
            points.push_back(glm::vec3(pos.x, 0, pos.y));
            int isInside = QuadTree::shapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight);
            if (isInside == 2) {
                this->bottomLeft->drawAll(frame);
            } else if (isInside == 1) {
                this->bottomLeft->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
            } else {
                glm::vec2 childeCenter(this->center.x - halfSize, this->center.y + halfSize);
                if (this->checkViewInside(childeCenter, viewPoints)) {
                    this->bottomLeft->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
                }
            }
        }

        if (this->bottomRight != NULL) {
            std::vector<glm::vec3> points;
            points.push_back(glm::vec3(this->center.x, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x + this->size, 0, this->center.y));
            points.push_back(glm::vec3(this->center.x + this->size, 0, this->center.y + this->size));
            points.push_back(glm::vec3(this->center.x, 0, this->center.y + this->size));
            glm::vec2 pos = this->calcBottomRightCenter();
            points.push_back(glm::vec3(pos.x, 0, pos.y));
            int isInside = QuadTree::shapeInsideView(points, pCloseLeft, pCloseRight, pFarLeft, pFarRight);
            if (isInside == 2) {
                this->bottomRight->drawAll(frame);
            } else if (isInside == 1) {
                this->bottomRight->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
            } else {
                glm::vec2 childeCenter(this->center.x + halfSize, this->center.y + halfSize);
                if (this->checkViewInside(childeCenter, viewPoints)) {
                    this->bottomRight->drawNodes(pCloseLeft, pCloseRight, pFarLeft, pFarRight, frame);
                }
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

        // std::cout << "\n";
        // std::cout << insideQuads[0];
        // std::cout << ":";
        // std::cout << insideQuads[1];
        // std::cout << ":";
        // std::cout << insideQuads[2];
        // std::cout << ":";
        // std::cout << insideQuads[3];
        // std::cout << "\n";

        if (insideQuads[0])
            this->insertGNode(&this->topLeft, gNode, this->calcTopLeftCenter());

        if (insideQuads[1])
            this->insertGNode(&this->topRight, gNode, this->calcTopRightCenter());

        if (insideQuads[2])
            this->insertGNode(&this->bottomLeft, gNode, this->calcBottomLeftCenter());

        if (insideQuads[3])
            this->insertGNode(&this->bottomRight, gNode, this->calcBottomRightCenter());
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

    bool QuadTreeNode::checkViewInside(glm::vec2 childeCenter, std::vector<glm::vec2> view) {
        float left = childeCenter.x - this->size/2.0f;
        float right = childeCenter.x + this->size/2.0f;
        float top = childeCenter.y - this->size/2.0f;
        float bottom = childeCenter.y + this->size/2.0f;
        for (int i = 0; i < view.size(); i++) {
            if (left <= view[i].x && right >= view[i].x && top <= view[i].y && bottom >= view[i].y) {
                return true;
            }
        }
        return false;
    }


    void QuadTreeNode::insertGNode(QuadTree** quadTreeNode, GraphicsNode* gNode, glm::vec2 childeCenter) {
        if (*quadTreeNode != NULL) {
            if ((*quadTreeNode)->isLeaf()) {
                GraphicsNode* oldGNode = ((QuadTreeLeaf*)(*quadTreeNode))->getGraphicsNode();
                (*quadTreeNode)->~QuadTree();
                (*quadTreeNode) = new QuadTreeNode(childeCenter, this->size/2.0f, this->depth + 1);
                ((QuadTreeNode*)(*quadTreeNode))->insertGraphicsNode(oldGNode);
                ((QuadTreeNode*)(*quadTreeNode))->insertGraphicsNode(gNode); 
            } else {
                ((QuadTreeNode*)(*quadTreeNode))->insertGraphicsNode(gNode);   
            }
        } else {
            (*quadTreeNode) = new QuadTreeLeaf(gNode);
        }
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
