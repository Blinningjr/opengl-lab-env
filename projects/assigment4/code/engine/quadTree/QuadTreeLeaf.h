#pragma once


#include "QuadTree.h"
#include "../GraphicsNode.h"


namespace Graphics3D {
	class QuadTreeLeaf : public QuadTree {
		public:

            QuadTreeLeaf(GraphicsNode* gNode);
            ~QuadTreeLeaf();

            std::vector<GraphicsNode*> getGraphicsNodes() override;

            GraphicsNode* getGraphicsNode();

            bool isLeaf() override; 

        private:

            GraphicsNode* gNode;
            
    };
} 
