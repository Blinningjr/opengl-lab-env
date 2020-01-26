#pragma once


#include "../GraphicsNode.h"
#include <vector>
#include <glm/glm.hpp>
#include "iostream"


namespace Graphics3D {
	class QuadTree {
		public:

            virtual void drawNodes(glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                glm::vec2 pFarLeft, glm::vec2 pFarRight, uint frame) = 0;

            virtual void drawAll(uint frame) = 0;
            
            virtual bool isLeaf() = 0;


            /**
             *  0 = Not inside.
             *  1 = Partly inside.
             *  2 = Whole shape inside.
            */
            static int shapeInsideView(std::vector<glm::vec3> points, glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                    glm::vec2 pFarLeft, glm::vec2 pFarRight) {
                uint numInside = 0;
                for (int i = 0; i < points.size(); i++) {
                    if (insideView(points[i], pCloseLeft, pCloseRight, pFarLeft, pFarRight)) {
                        numInside += 1;
                    }
                }
                if (numInside == points.size()) {
                    return 2;
                } else if (numInside > 0) {
                    return 1;
                } else {
                    return 0;
                }
            }

            static bool insideView(glm::vec3 gNodePos, glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                    glm::vec2 pFarLeft, glm::vec2 pFarRight) {

                glm::vec2 pos(gNodePos.x, gNodePos.z);

                return (toTheRightOfLine(pCloseLeft - pFarLeft, pos - pFarLeft)
                        && toTheRightOfLine(pFarLeft - pFarRight, pos - pFarRight)
                        && toTheRightOfLine(pFarRight - pCloseRight, pos - pCloseRight)
                        && toTheRightOfLine(pCloseRight - pCloseLeft, pos - pCloseLeft));
            }

            static bool toTheRightOfLine(glm::vec2 line, glm::vec2 point) {
                glm::vec3 v(line.x, 0, line.y);
                glm::vec3 u(point.x, 0, point.y);
                return glm::cross(v, u).y >= 0;
            }
            
    };
} 
