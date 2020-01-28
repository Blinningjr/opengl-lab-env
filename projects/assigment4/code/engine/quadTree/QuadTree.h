#pragma once


#include "../node/GraphicsNode.h"
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
             *  Checks if all points are in the quad formed by the ohter points
            */
            static bool wholeShapeInsideView(std::vector<glm::vec3> points, glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                    glm::vec2 pFarLeft, glm::vec2 pFarRight) {
                for (int i = 0; i < points.size(); i++) {
                    if (!insideView(points[i], pCloseLeft, pCloseRight, pFarLeft, pFarRight))
                        return false;
                }
                return true;
            }


            /**
             *  Checks if at least one point of points is in the quad formed by the ohter points
            */
            static bool shapePartlyInsideView(std::vector<glm::vec3> points, glm::vec2 pCloseLeft, glm::vec2 pCloseRight,
                    glm::vec2 pFarLeft, glm::vec2 pFarRight) {
                for (int i = 0; i < points.size(); i++) {
                    if (insideView(points[i], pCloseLeft, pCloseRight, pFarLeft, pFarRight))
                        return true;
                }
                return false;
            }


            /**
             *  Checks if at least one point of points is in the quad formed by the ohter points
            */
            static bool shapeInstersectsView(std::vector<glm::vec3> points, std::vector<glm::vec2> viewPoints) {
                for (int i = 0; i < viewPoints.size(); i++) {
                    for (int j = 0; j < points.size(); j++) {
                        glm::vec3 point1 = points[j];
                        glm::vec3 point2 = points[(j + 1)%points.size()];
                        if (linesIntersects(viewPoints[i], viewPoints[(i + 1)%viewPoints.size()],
                            glm::vec2(point1.x, point1.z), glm::vec2(point2.x, point2.z)))
                            return true;
                    }
                }
                return false;
            }


            /**
             *  Check if lines intersect.
            */
            static bool linesIntersects(glm::vec2 l1p1, glm::vec2 l1p2, glm::vec2 l2p1, glm::vec2 l2p2) {
                return (toTheRightOfLine(l1p2 - l1p1, l2p1 - l1p1) != toTheRightOfLine(l1p2 - l1p1, l2p2 - l1p1))
                    && (toTheRightOfLine(l2p2 - l2p1, l1p1 - l2p1) != toTheRightOfLine(l2p2 - l2p1, l1p2 - l2p1));
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
