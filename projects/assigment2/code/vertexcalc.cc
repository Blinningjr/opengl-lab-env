#include "vertexcalc.h"
#include <iostream>
#include <string>
#include <GL/glew.h>

namespace Triangulation3d {

    /**
     *  Creates a reader with no read points 
     */
    VertexCalc::VertexCalc() {
        this->pointsLength = 0;
        this->points = new VertexCalc::Point[this->pointsLength];
        this->convexHullLength = 0;
        this->convexHull = new VertexCalc::Point[this->convexHullLength];
    }


    /**
     *  Deletes all values made by this class. 
     */
    VertexCalc::~VertexCalc() {
        delete[] this->points;
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

            this->points[i].r = 0;
            this->points[i].g = 0;
            this->points[i].b = 0;
            this->points[i].a = 1;
        }
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
        this->points = new Point[pointsLength];

        for (int i = 0; i < numPoints; i++) {
            this->points[i].x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)) - 1.0f;
            this->points[i].y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)) - 1.0f;
            this->points[i].z = -1;

            this->points[i].r = 0;
            this->points[i].g = 0;
            this->points[i].b = 0;
            this->points[i].a = 1;
        }
    }


    /**
     * Calculates the convex hull of the array points and stores it in convexHull.
     */
    void VertexCalc::calcConvexHull() {
        // delete[]  this->convexHull;
        // this->convexHullLength = 0;

        // int numPoints = this->pointsLength/7;
        // if (numPoints == 3) {
        //     this->convexHullLength = this->pointsLength;
        //     this->convexHull = new GLfloat[this->convexHullLength];
        //     for (int i = 0; i <  this->pointsLength; i++) {
        //         this->convexHull[i] = this->points[i];
        //     }
        //     return;
        // }

    }


    void VertexCalc::sort(VertexCalc::Point* points, int length) {
        // VertexCalc::Point ps[length/7];
        // for (int i = 0; i < length/7; i++) {
        //     VertexCalc::Point p;
        //     p.x = points[0 + i * 7];
        //     p.y = points[1 + i * 7];
        //     p.z = points[2 + i * 7];
            
        //     p.r = points[3 + i * 7];
        //     p.g = points[4 + i * 7];
        //     p.b = points[5 + i * 7];
        //     p.a = points[6 + i * 7];
        // }
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
}
