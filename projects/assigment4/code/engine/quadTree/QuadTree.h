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
             *  Cheacks if points are all inside or partl inside and if it is not inside the box formed by the 
             *  other points.
             * 
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


            /**
             * returns true if  point is inside the  quad formed by the other points.
            */
            static bool insideView(glm::vec3 point, glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                    glm::vec2 pFarLeft, glm::vec2 pFarRight) {

                glm::vec2 pos(point.x, point.z);
                return (toTheRightOfLine(pCloseLeft - pFarLeft, pos - pFarLeft)
                        && toTheRightOfLine(pFarLeft - pFarRight, pos - pFarRight)
                        && toTheRightOfLine(pFarRight - pCloseRight, pos - pCloseRight));
            }


            /**
             * returns true if the point is to the right of the line or on the line.
            */
            static bool toTheRightOfLine(glm::vec2 line, glm::vec2 point) {
                glm::vec3 v(line.x, 0, line.y);
                glm::vec3 u(point.x, 0, point.y);
                // std::cout << "\n";
                // std::cout << "start \n";
                // std::cout << line.x;
                // std::cout << " : ";
                // std::cout << line.y;
                // std::cout << "\n";
                // std::cout << point.x;
                // std::cout << " : ";
                // std::cout << point.y;
                // std::cout << "\n";
                // std::cout << glm::cross(v, u).y;
                // std::cout << "\n";
                return glm::cross(v, u).y >= 0;
            }
            
    };
} 
