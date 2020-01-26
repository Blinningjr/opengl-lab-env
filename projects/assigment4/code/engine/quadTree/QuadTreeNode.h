#pragma once


#include "QuadTree.h"
#include "../GraphicsNode.h"
#include "glm/vec2.hpp"
#include <memory>


namespace Graphics3D {
	class QuadTreeNode : public QuadTree {
		public:

            QuadTreeNode(glm::vec2 center, float size, uint depth);
            QuadTreeNode(glm::vec2 center, float size, GraphicsNode* gNode);
            ~QuadTreeNode();

            void drawNodes(glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                glm::vec2 pFarLeft, glm::vec2 pFarRight, uint frame) override;
            void drawAll(uint frame) override;
       

            bool isLeaf() override; 

            void insertGraphicsNode(GraphicsNode* gNode);

        private:

            glm::vec2 center;
            float size;

            QuadTree* topLeft;
            QuadTree* topRight;
            QuadTree* bottomLeft;
            QuadTree* bottomRight;

            bool* checkInsideWhichQuads(bool* insideQuads, GraphicsNode* gNode);

            glm::vec2 calcTopLeftCenter();
            glm::vec2 calcTopRightCenter();
            glm::vec2 calcBottomLeftCenter();
            glm::vec2 calcBottomRightCenter();

            uint depth;
            
    };
} 
