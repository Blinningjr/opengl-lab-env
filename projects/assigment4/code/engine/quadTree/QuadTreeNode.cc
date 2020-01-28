#include "QuadTreeNode.h"
#include "QuadTreeLeaf.h"
#include "iostream"


namespace Graphics3D {


    QuadTreeNode::QuadTreeNode(glm::vec2 center, float size, uint depth) {
        this->center = center;
        this->size = size;    
        this->depth = depth;

        this->init();
    }


    QuadTreeNode::QuadTreeNode(glm::vec2 center, float size, GraphicsNode* gNode) {
        this->center = center;
        this->size = size;
    
        this->init();

        this->insertGraphicsNode(gNode);
    }


    QuadTreeNode::~QuadTreeNode() {
        
    }

    
    /**
     *  Draws all objects in the subtree that are inside the view.
    */
    void QuadTreeNode::drawNodes(glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                glm::vec2 pFarLeft, glm::vec2 pFarRight, uint frame) {

        std::vector<glm::vec2> viewPoints;
        viewPoints.push_back(pCloseLeft);
        viewPoints.push_back(pCloseRight);
        viewPoints.push_back(pFarLeft);
        viewPoints.push_back(pFarRight);
        // viewPoints.push_back(pFarLeft + (pFarRight - pFarLeft)/2.0f + (pFarRight - pFarLeft)/2.0f);

        float halfSize = this->size/2.0f;

        if (this->topLeft != NULL)
            this->drawNode(&this->topLeft, this->topLeftPoints, viewPoints, this->calcTopLeftCenter(), frame);

        if (this->topRight != NULL)
            this->drawNode(&this->topRight, this->topRightPoints, viewPoints, this->calcTopRightCenter(), frame);

        if (this->bottomLeft != NULL)
            this->drawNode(&this->bottomLeft, this->bottomLeftPoints, viewPoints, this->calcBottomLeftCenter(), frame);

        if (this->bottomRight != NULL)
            this->drawNode(&this->bottomRight, this->bottomRightPoints, viewPoints, this->calcBottomRightCenter(), frame);
    }


    /**
     *  Draws all objects in this subtree.
    */
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
       

    /**
     *  Returns true if this is a leaf.
    */
    bool QuadTreeNode::isLeaf() {
        return false;
    }


    /**
     *  Inserts a GraphicsNode into this subtree.
    */
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


    /**
     *  Checks which subtree GraphicsNode position is inside.
     *  Returns a pointer to a list of four booles, representing the four subtrees.
    */
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


    /**
     *  Checks if the view is inside the quad this node represents.
    */
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


    /**
     *  Helper function for inserting a GraphicsNode into a subtree.
    */
    void QuadTreeNode::insertGNode(QuadTree** quadTree, GraphicsNode* gNode, glm::vec2 childeCenter) {
        if (*quadTree != NULL) {
            if ((*quadTree)->isLeaf()) {
                GraphicsNode* oldGNode = ((QuadTreeLeaf*)(*quadTree))->getGraphicsNode();
                (*quadTree)->~QuadTree();
                (*quadTree) = new QuadTreeNode(childeCenter, this->size/2.0f, this->depth + 1);
                ((QuadTreeNode*)(*quadTree))->insertGraphicsNode(oldGNode);
                ((QuadTreeNode*)(*quadTree))->insertGraphicsNode(gNode); 
            } else {
                ((QuadTreeNode*)(*quadTree))->insertGraphicsNode(gNode);   
            }
        } else {
            (*quadTree) = new QuadTreeLeaf(gNode);
        }
    }


    /**
     *  Helper function for checking is subtree is suppose to be drawn or not.
    */
    void QuadTreeNode::drawNode(QuadTree** quadTreeNode, std::vector<glm::vec3> points, std::vector<glm::vec2> viewPoints,
            glm::vec2 childeCenter, uint frame) {
        if (QuadTree::wholeShapeInsideView(points, viewPoints[0], viewPoints[1], viewPoints[2], viewPoints[3])) {
            (*quadTreeNode)->drawAll(frame);
        } else if (QuadTree::shapeInstersectsView(points, viewPoints)) {
            (*quadTreeNode)->drawNodes(viewPoints[0], viewPoints[1], viewPoints[2], viewPoints[3], frame);
        } else if (this->checkViewInside(childeCenter, viewPoints)) {
            (*quadTreeNode)->drawNodes(viewPoints[0], viewPoints[1], viewPoints[2], viewPoints[3], frame);
        }
    }


    /**
     *  Helpter function for initializing class variables.
    */
    void QuadTreeNode::init() {
        this->topLeft = NULL;
        this->topRight = NULL;
        this->bottomLeft = NULL;
        this->bottomRight = NULL;

        this->topLeftPoints.push_back(glm::vec3(this->center.x - this->size, 0, this->center.y -this->size));
        this->topLeftPoints.push_back(glm::vec3(this->center.x, 0, this->center.y - this->size));
        this->topLeftPoints.push_back(glm::vec3(this->center.x, 0, this->center.y));
        this->topLeftPoints.push_back(glm::vec3(this->center.x - this->size, 0, this->center.y));
        // glm::vec2 topLeftCenter = this->calcTopLeftCenter();
        // this->topLeftPoints.push_back(glm::vec3(topLeftCenter.x, 0, topLeftCenter.y));

        this->topRightPoints.push_back(glm::vec3(this->center.x, 0, this->center.y - this->size));
        this->topRightPoints.push_back(glm::vec3(this->center.x + this->size, 0, this->center.y - this->size));
        this->topRightPoints.push_back(glm::vec3(this->center.x + this->size, 0, this->center.y));
        this->topRightPoints.push_back(glm::vec3(this->center.x, 0, this->center.y));
        // glm::vec2 topRightCenter = this->calcTopRightCenter();
        // this->topRightPoints.push_back(glm::vec3(topRightCenter.x, 0, topRightCenter.y));

        this->bottomLeftPoints.push_back(glm::vec3(this->center.x - this->size, 0, this->center.y));
        this->bottomLeftPoints.push_back(glm::vec3(this->center.x, 0, this->center.y));
        this->bottomLeftPoints.push_back(glm::vec3(this->center.x, 0, this->center.y + this->size));
        this->bottomLeftPoints.push_back(glm::vec3(this->center.x - this->size, 0, this->center.y + this->size));
        // glm::vec2 bottomLeftCenter = this->calcBottomLeftCenter();
        // this->bottomLeftPoints.push_back(glm::vec3(bottomLeftCenter.x, 0, bottomLeftCenter.y));

        this->bottomRightPoints.push_back(glm::vec3(this->center.x, 0, this->center.y));
        this->bottomRightPoints.push_back(glm::vec3(this->center.x + this->size, 0, this->center.y));
        this->bottomRightPoints.push_back(glm::vec3(this->center.x + this->size, 0, this->center.y + this->size));
        this->bottomRightPoints.push_back(glm::vec3(this->center.x, 0, this->center.y + this->size));
        // glm::vec2 bottomRightCenter = this->calcBottomRightCenter();
        // this->bottomRightPoints.push_back(glm::vec3(bottomRightCenter.x, 0, bottomRightCenter.y));
    }


    /**
     *  Calculates the center of the top left quad.
    */
    glm::vec2 QuadTreeNode::calcTopLeftCenter() {
        return glm::vec2(this->center.x - this->size/2.0f, this->center.y - this->size/2.0f);
    }


    /**
     *  Calculates the center of the top right quad.
    */
    glm::vec2 QuadTreeNode::calcTopRightCenter() {
        return glm::vec2(this->center.x + this->size/2.0f, this->center.y - this->size/2.0f);
    }


    /**
     *  Calculates the center of the bottom left quad.
    */
    glm::vec2 QuadTreeNode::calcBottomLeftCenter() {
        return glm::vec2(this->center.x - this->size/2.0f, this->center.y + this->size/2.0f);
    }


    /**
     *  Calculates the center of the bottom right quad.
    */
    glm::vec2 QuadTreeNode::calcBottomRightCenter() {
        return glm::vec2(this->center.x + this->size/2.0f, this->center.y + this->size/2.0f);
    }
}
