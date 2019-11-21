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
        this->points = new GLfloat[this->pointsLength];
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
        this->pointsLength = numCords * 7;
        this->points = new GLfloat[pointsLength];

        for (int i = 0; i < numCords; i++) {
            this->points[0 + i * 7] = tBuf[0 + i * 2];
            this->points[1 + i * 7] = tBuf[1 + i * 2];
            this->points[2 + i * 7] = -1;

            this->points[3 + i * 7] = 0;
            this->points[4 + i * 7] = 0;
            this->points[5 + i * 7] = 0;
            this->points[6 + i * 7] = 1;
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
        this->pointsLength = numPoints * 7;
        this->points = new GLfloat[pointsLength];

        for (int i = 0; i < numPoints; i++) {
            this->points[0 + i * 7] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)) - 1.0f;
            this->points[1 + i * 7] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)) - 1.0f;
            this->points[2 + i * 7] = -1;

            this->points[3 + i * 7] = 0;
            this->points[4 + i * 7] = 0;
            this->points[5 + i * 7] = 0;
            this->points[6 + i * 7] = 1;
        }
    }


    /**
     * Calculates the convex hull of the array points and stores it in convexHull.
     */
    void VertexCalc::calcConvexHull() {

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
    GLfloat* VertexCalc::getPoints() {
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
	GLfloat* VertexCalc::getConvexHull() {
        return this->convexHull;
    }
}
