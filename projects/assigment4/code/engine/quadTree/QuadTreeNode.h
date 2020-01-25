#pragma once


#include "QuadTree.h"
#include "../GraphicsNode.h"
#include "glm/vec2.hpp"
#include <memory>


namespace Graphics3D {
	class QuadTreeNode : public QuadTree {
		public:

            QuadTreeNode(glm::vec2 center, float size);
            QuadTreeNode(glm::vec2 center, float size, GraphicsNode* gNode);
            ~QuadTreeNode();

            std::vector<GraphicsNode*> getGraphicsNodes() override;

            bool isLeaf() override; 

            void insertGraphicsNode(GraphicsNode* gNode);

        private:

            glm::vec2 center;
            float size;

            QuadTree* topLeft;
            QuadTree* topRight;
            QuadTree* bottomLeft;
            QuadTree* bottomRight;

            bool* checkInsideWhichQuads(GraphicsNode* gNode);

            glm::vec2 calcTopLeftCenter();
            glm::vec2 calcTopRightCenter();
            glm::vec2 calcBottomLeftCenter();
            glm::vec2 calcBottomRightCenter();
   
            
    };
} 
