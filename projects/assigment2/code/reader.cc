#include "reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>

namespace Triangulation3d {

    /**
     *  Creates a reader with no read points 
     */
    Reader::Reader() {
        this->pointsLength = 0;
        this->points = new GLfloat[this->pointsLength];
    }


    /**
     *  Deletes all values made by this class. 
     */
    Reader::~Reader() {
        delete[] this->points;
    }


    /**
     *  Reads points from file and updates the class variables with the read data.
     *  file_path: is the path to the file.
     */
    void Reader::readPoints(std::string file_path) {
        std::ifstream infile(file_path);
        int numCords;
        infile >> numCords;
        // std::cout << numCords;
        // std::cout << "\n";
        this->pointsLength = numCords * 2;
        delete[] this->points;
        this->points = new GLfloat[this->pointsLength];

        for (int i = 0; i < numCords; i++) {
            GLfloat x, y;
            infile >> x >> y;
            // std::cout << x;
            // std::cout << " ";
            // std::cout << y;
            // std::cout << "\n";
            this->points[0 + i * 2] = x;
            this->points[1 + i * 2] = y;
        }
    }


    /**
     *  Gets the length of the points array.
     */
    int Reader::getPointsLength() {
        return this->pointsLength;
    }


    /**
     *  Gets a pointer to the array with read points. 
     */
    GLfloat* Reader::getPoints() {
        return this->points;
    }
}
