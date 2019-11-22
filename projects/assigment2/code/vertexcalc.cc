#include "vertexcalc.h"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <algorithm>
#include <math.h>  

namespace Triangulation3d {

    /**
     *  Creates a reader with no read points 
     */
    VertexCalc::VertexCalc() {
        this->pointsLength = 0;
        this->points = new Point[this->pointsLength];
        this->convexHullLength = 0;
        this->convexHull = new Point[this->convexHullLength];
        this->triangulationLength = 0;
        this->triangulation = new Triangle[this->triangulationLength];
        
        this->pickedC.x = 0;
        this->pickedC.y = 0;
        this->pickedC.z = -1;

        this->pickedC.r = 0;
        this->pickedC.g = 0;
        this->pickedC.b = 1;
        this->pickedC.a = 1;
    }


    /**
     *  Deletes all values made by this class. 
     */
    VertexCalc::~VertexCalc() {
        delete[] this->points;
        delete[] this->convexHull;
        delete[] this->triangulation;
    }


    /**
     *  Reads points form a file at the file path filePath. 
     */
    void VertexCalc::ReadPoints(std::string filePath) {
        this->reader.ReadPoints(filePath);
        int numCords = this->reader.getPointsLength()/2;
        GLfloat* tBuf = this->reader.getPoints();

        delete[] this->points;
        this->pointsLength = numCords;
        this->points = new Point[this->pointsLength];

        for (int i = 0; i < numCords; i++) {
            this->points[i].x = tBuf[0 + i * 2];
            this->points[i].y = tBuf[1 + i * 2];
            this->points[i].z = -1;

            this->points[i].r = 1;
            this->points[i].g = 1;
            this->points[i].b = 1;
            this->points[i].a = 1;
        }
        std::sort(this->points, this->points + this->pointsLength);
        this->calcTriangulation();
    }


    /**
     *  Generates random points of size numPoints.
     *  NOTE: If (numPoints < 3) numPoints = 3;
     */
    void VertexCalc::GenRandomPoints(int numPoints) {
        if (numPoints < 3) {
            numPoints = 3;
        }

        delete[] this->points;
        this->pointsLength = numPoints;
        this->points = new Point[this->pointsLength];

        for (int i = 0; i < numPoints; i++) {
            this->points[i].x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.8f)) - 0.9f;
            this->points[i].y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.8f)) - 0.9f;
            this->points[i].z = -1;

            this->points[i].r = 1;
            this->points[i].g = 1;
            this->points[i].b = 1;
            this->points[i].a = 1;
        }
        std::sort(this->points, this->points + this->pointsLength);

        GLfloat onLine = (this->points[numPoints - 1].x - this->points[0].x)/(this->points[numPoints - 1].y - this->points[0].y);
        bool ok = false;
        for (int i = 0; i < numPoints - 1; i++) {
            if (this->points[i].x == this->points[i + 1].x && this->points[i].y == this->points[i + 1].y) {
                this->GenRandomPoints(numPoints);
                return;
            } 
            GLfloat line = (this->points[i + 1].x - this->points[0].x)/(this->points[i + 1].y - this->points[0].y);
            if (line != onLine) {
                ok = true;
            }
        }
        if (!ok) {
            this->GenRandomPoints(numPoints);
            return;
        }

        this->calcTriangulation();
    }


    /**
     * Calculates the convex hull of the array points and stores it in convexHull.
     * Andrew's algorithm.
     */
    void VertexCalc::calcConvexHull() {
        delete[]  this->convexHull;
        this->convexHullLength = 0;

        if (this->pointsLength == 3) {
            this->convexHullLength = this->pointsLength;
            this->convexHull = new Point[this->convexHullLength];
            for (int i = 0; i <  this->pointsLength; i++) {
                this->convexHull[i] = this->points[i];
            }
            return;
        }

        int lLen = 0;
        Point l[this->pointsLength];

        for (int i = 0; i < this->pointsLength; i++) {
            while (lLen >= 2 && this->crossProduct(l[lLen-2], l[lLen-1], this->points[i]) <= 0) {
                lLen--;
            }
            l[lLen] = this->points[i];
            lLen += 1;
	    }

        int uLen = 0;
        Point u[this->pointsLength];

        for (int i = this->pointsLength - 1; i >= 0; i--) {
            while (uLen >= 2 && this->crossProduct(u[uLen-2], u[uLen-1], this->points[i]) <= 0) {
                uLen--;
            }
            u[uLen] = this->points[i];
            uLen += 1;
	    }

        this->convexHullLength = lLen + uLen - 2;
        this->convexHull = new Point[this->convexHullLength];
        
        for (int i = 0; i < lLen - 1; i++) {
            this->convexHull[i] = l[i];
        }
        for (int i = 0; i < uLen - 1 ; i++) {
            this->convexHull[i + lLen - 1] = u[i];
        }
    }


    /**
     *  Calculates the cross product of bxc relative to the point a.
     *  In other words (b-a)x(c-a).
     */
    float VertexCalc::crossProduct(Point a, Point b, Point c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }


    void VertexCalc::calcTriangulation() {
        this->calcConvexHull();
        this->pickC();
        delete[] this->triangulation;
        this->triangulationLength = 1;
        this->triangulation = new Triangle[this->triangulationLength];
        this->triangulation[0].p1 = this->points[0];
        this->triangulation[0].p2 = this->points[1];
        this->triangulation[0].p3 = this->points[2];
    }


    /**
     *  Picks the vertex closses to the center of the box created by the largest and smalles x and y value.
     */
    void VertexCalc::pickC() {
        this->pickedC.x = this->points[0].x;
        this->pickedC.y = this->points[0].y;
        GLfloat cx = 0;
        GLfloat cy = 0;

        cx = this->points[0].x + (this->points[this->pointsLength - 1].x - this->points[0].x)/2;

        GLfloat maxY = this->convexHull[0].y;
        GLfloat minY = this->convexHull[0].y;
        for (int i = 1; i < this->convexHullLength; i ++) {
            if (maxY < this->convexHull[i].y) {
                maxY = this->convexHull[i].y;
            } else if (minY > this->convexHull[i].y) {
                minY = this->convexHull[i].y;
            }
        }
        cy = minY + (maxY - minY)/2;

        for (int i = 1; i < this->pointsLength; i++) {
            if (pow(this->pickedC.x - cx, 2) + pow(this->pickedC.y - cy, 2) > pow(this->points[i].x - cx, 2) + pow(this->points[i].y - cy, 2)) {
                this->pickedC.x = this->points[i].x;
                this->pickedC.y = this->points[i].y;
            }
        }
    }


    /**
     *  Gets the length of the points array.
     */
    int VertexCalc::getPointsLength() {
        return this->pointsLength;
    }


    /**
     *  Gets a pointer to the array with read points. 
     */
    VertexCalc::Point* VertexCalc::getPoints() {
        return this->points;
    }


    /**
     *  Gets the length of the convecHull array.
     */
    int VertexCalc::getConvexHullLength() {
        return this->convexHullLength;
    }


    /**
     *  Gets a pointer to the array with the convex hull. 
     */
	VertexCalc::Point* VertexCalc::getConvexHull() {
        return this->convexHull;
    }


    /**
     *  Gets the length of the triangulation array.
     */
    int VertexCalc::getTriangulationLength() {
        return this->triangulationLength;
    }


    /**
     *  Gets a pointer to the array with the triangulation. 
     */
	VertexCalc::Triangle* VertexCalc::getTriangulation() {
        return this->triangulation;
    }


    /**
     *  Get the point picked as the center of the points 
     */
    VertexCalc::Point VertexCalc::getPickedC() {
        return this->pickedC;
    }
}
