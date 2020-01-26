#pragma once


#include "QuadTree.h"
#include "../GraphicsNode.h"


namespace Graphics3D {
	class QuadTreeLeaf : public QuadTree {
		public:

            QuadTreeLeaf(GraphicsNode* gNode);
            ~QuadTreeLeaf();

            void drawNodes(glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                glm::vec2 pFarLeft, glm::vec2 pFarRight, uint frame) override;

            void drawAll(uint frame) override;
            
            GraphicsNode* getGraphicsNode();

            bool isLeaf() override; 

        private:

            GraphicsNode* gNode;

            
    };
} 
