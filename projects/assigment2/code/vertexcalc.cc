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
        Node* n = new Node();
        this->tree = n;
        this->leaf = new Leaf();
    }


    /**
     *  Deletes all values made by this class. 
     */
    VertexCalc::~VertexCalc() {
        delete[] this->points;
        delete[] this->convexHull;
        this->deleteTree(this->tree);
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
        int cLength = this->convexHullLength;
        // if (this->pickedCOnHull()) {
        //     cLength -= 1;
        // }
        this->triangulationLength = cLength;
        this->triangulation = this->calcTriangles(this->convexHull, this->convexHullLength, this->pickedC);

        this->deleteTree(this->tree);
        this->tree = this->createTree(this->convexHull, this->convexHullLength);
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


    
    VertexCalc::Triangle* VertexCalc::calcTriangles(Point* ps, int length, Point v) {
        Triangle* triangles = new Triangle[length];
        for (int i = 0; i < length - 1; i++) {
            triangles[i].p1 = ps[i];
            triangles[i].p2 = v;
            triangles[i].p3 = ps[i + 1];
        }

        triangles[length-1].p1 = ps[length-1];
        triangles[length-1].p2 = v;
        triangles[length-1].p3 = ps[0];

        Leaf* pl = new Leaf();
        pl->triangle = triangles[0];
        this->leaf = pl;
        for (int i = 1; i < length; i++) {
            Leaf* l = new Leaf();
            l->triangle = triangles[i];
            pl->ll = l;
            l->rl = pl;
            pl = l;
        }
        pl->ll = this->leaf;
        this->leaf->rl = pl;

        return triangles;
    }

    VertexCalc::Node* VertexCalc::createTree(Point* ps, int length) {
        Node* node = new Node();
        Edge* edge = new Edge();
        edge->p1 = this->pickedC;
        edge->p2 = ps[length/2];
        
        Point tps[length-1];
        int j = 0;
        for (int i = 0; i < length; i++) {
            if (i == length/2) {
                j -= 1;
            } else {
                tps[j] = ps[i];
            }
            j += 1;
        }

        int lpsLength = 0;
        Point* lps = new Point[length];

        int rpsLength = 0;
        Point* rps = new Point[length];

        for (int i = 0; i < length-1; i++) {
            if (this->crossProduct(edge->p1, edge->p2, tps[i]) <= 0) {
                rps[rpsLength] = tps[i];
                rpsLength += 1;
            } else {
                lps[lpsLength] = tps[i];
                lpsLength += 1;
            }
        }

        BNode* bNode = new BNode();
        bNode->e = edge;

        if (lpsLength == 0) {
            bNode->lst = this->findTriangle(edge, true);
        } else {
            bNode->lst = createTree(lps, lpsLength);
        }
        
        if (rpsLength == 0) {
            bNode->rst = this->findTriangle(edge, false);
        } else {
            bNode->rst = createTree(rps, rpsLength);
        }

        delete[] lps;
        delete[] rps;
        
        return node;
    }

    VertexCalc::Node* VertexCalc::findTriangle(Edge* edge, bool left) {
        Node* node = new Node();

        Leaf* l = this->leaf;
        while (true) {
            if (left && l->triangle.p1 == edge->p2) {
                node->l = l;
                return node;
            } else if (!left && l->triangle.p3 == edge->p2) {
                node->l = l;
                return node;
            }
            l = l->ll;
        }
    }

    void VertexCalc::deleteTree(Node* node) {
        if (node->l != NULL) {
            delete node->l;
        }
        if (node->bn != NULL) {
            this->deleteTree(node->bn->lst);
            this->deleteTree(node->bn->rst);
            delete node->bn->e;
            delete node->bn;
        }
        if (node->t != NULL) {
            this->deleteTree(node->t->lst);
            this->deleteTree(node->t->mst);
            this->deleteTree(node->t->rst);
            delete node->t->e1;
            delete node->t->e2;
            delete node->t->e3;
        }
        delete node->l;
        delete node->bn;
        delete node->t;
        delete node;
    }

    bool VertexCalc::pickedCOnHull() {
        for (int i = 0; i < this->convexHullLength; i++) {
            if (this->pickedC == this->convexHull[i]) {
                return true;
            }
        }
        return false;
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
