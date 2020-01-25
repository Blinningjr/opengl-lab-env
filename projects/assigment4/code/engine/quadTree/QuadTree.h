#pragma once


#include "../GraphicsNode.h"
#include <vector>


namespace Graphics3D {
	class QuadTree {
		public:

            virtual std::vector<GraphicsNode*> getGraphicsNodes() = 0;
            
            virtual bool isLeaf() = 0;

        private:

            
    };
} 
