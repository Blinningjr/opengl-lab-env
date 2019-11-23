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

        int pos = 0;
        Point rest[this->pointsLength - this->convexHullLength];

        for (int i = 0; i < this->pointsLength; i++) {
            bool add = true;
            for (int j = 0; j < this->convexHullLength; j++) {
                if (this->convexHull[j] == this->points[i]) {
                    add = false;
                    break;
                }
            }
            if (add) {
                rest[pos] = this->points[i];
                pos += 1;
            }
        }
        for (int i = 0; i < this->pointsLength - this->convexHullLength; i++) {
            this->insertPoint(rest[i], this->tree);
        }
        // this->insertPoint(rest[0], this->tree);
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
            bNode->lst = this->findLeaf(edge, true);
        } else {
            bNode->lst = createTree(lps, lpsLength);
        }
        
        if (rpsLength == 0) {
            bNode->rst = this->findLeaf(edge, false);
        } else {
            bNode->rst = createTree(rps, rpsLength);
        }

        node->bn = bNode;

        delete[] lps;
        delete[] rps;
        
        return node;
    }

    VertexCalc::Node* VertexCalc::findLeaf(Edge* edge, bool left) {
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


    void VertexCalc::insertPoint(Point p, Node* node0) {
        Node* node = this->getLeaf(p, node0);
        if (node->l != NULL) {
            
            Leaf* l = node->l;
            if (l == this->leaf) {
                this->leaf = l->ll;
            }
            node->l = NULL;
   
            Triangle t1;
            t1.p1 = l->triangle.p2;
            t1.p2 = p;
            t1.p3 = l->triangle.p1;

            Triangle t2;
            t2.p1 = l->triangle.p1;
            t2.p2 = p;
            t2.p3 = l->triangle.p3;

            Triangle t3;
            t3.p1 = l->triangle.p3;
            t3.p2 = p;
            t3.p3 = l->triangle.p2;

            
            Trenary* trenary = new Trenary();
            trenary->v = p;
            trenary->e1 = new Edge();
            trenary->e1->p1 = p;
            trenary->e1->p2 = l->triangle.p1;

            trenary->e2 = new Edge();
            trenary->e2->p1 = p;
            trenary->e2->p2 = l->triangle.p2;

            trenary->e3 = new Edge();
            trenary->e3->p1 = p;
            trenary->e3->p2 = l->triangle.p3;

            trenary->lst = new Node();
            trenary->lst->l = new Leaf();
            trenary->lst->l->triangle = t1;

            trenary->mst = new Node();
            trenary->mst->l = new Leaf();
            trenary->mst->l->triangle = t2;

            trenary->rst = new Node();
            trenary->rst->l = new Leaf();
            trenary->rst->l->triangle = t3;

            trenary->lst->l->ll = trenary->rst->l;
            trenary->lst->l->ml = l->ll;
            trenary->lst->l->rl = trenary->mst->l;

            trenary->mst->l->ll = trenary->lst->l;
            trenary->mst->l->ml = l->ml;
            trenary->mst->l->rl = trenary->rst->l;

            trenary->rst->l->ll = trenary->mst->l;
            trenary->rst->l->ml = l->rl;
            trenary->rst->l->rl = trenary->lst->l;
            
            this->insertLeafPointer(l->ll, l, trenary->lst->l);
            this->insertLeafPointer(l->ml, l, trenary->mst->l);
            this->insertLeafPointer(l->rl, l, trenary->rst->l);

            node->t = trenary;

            int pos = 0;
            int length = this->triangulationLength + 2;
            Triangle* triangle = new Triangle[length];
            for (int i = 0; i < this->triangulationLength; i++) {
                if (!(l->triangle == this->triangulation[i])) {
                    triangle[pos] = this->triangulation[i];
                    pos += 1;
                }
            }
            triangle[pos + 0] = t1;
            triangle[pos + 1] = t2;
            triangle[pos + 2] = t3;

            delete[] this->triangulation;
            this->triangulationLength = length;
            this->triangulation = triangle;
            delete l;
        } else {
            std::cout << "Error insertPoint \n";
        }
    }

    
    VertexCalc::Node* VertexCalc::getLeaf(Point p, Node* node) {
        if (node->l != NULL) {
            return node;
        } else if (node->bn != NULL) {
            if (this->crossProduct(node->bn->e->p1, node->bn->e->p2, p) < 0) {
                return this->getLeaf(p, node->bn->rst);
            } else if (this->crossProduct(node->bn->e->p1, node->bn->e->p2, p) > 0) {
                return this->getLeaf(p, node->bn->lst);
            } else {
                std::cout << "On line \n";
            }
        } else if (node->t != NULL) {
            Trenary* t = node->t;
            if (this->isInsideEdges(t->e1, t->e2, p)) {
                return this->getLeaf(p, t->lst);
            } else if (this->isInsideEdges(t->e3, t->e1, p)) {
                return this->getLeaf(p, t->mst);
            } else if (this->isInsideEdges(t->e2, t->e3, p)) {
                return this->getLeaf(p, t->rst);
            } else {
                std::cout << "Error Trenary getLeaf \n";
            }
        } else {
            std::cout << "Error Empty Node \n";
        }
        return node;
    }

    void VertexCalc::insertLeafPointer(Leaf* l0, Leaf* l1, Leaf* l2) {
        if (l0 == NULL) {
            // std::cout << "NULL insertLeafPointer \n";
            return;
        }

        if (l0->ll != NULL) {
            if (l0->ll->triangle == l1->triangle) {
                l0->ll = l2;
                return;
            } 
        }

        if (l0->ml != NULL) {
            if (l0->ml->triangle == l1->triangle) {
                l0->ml = l2;
                return;
            } 
        }
        if (l0->rl != NULL) {
            if (l0->rl->triangle == l1->triangle) {
                l0->rl = l2;
                return;
            } 
        }

        std::cout << "Error insertLeafPointer \n";
    }

    bool VertexCalc::isInsideEdges(Edge* e0, Edge* e1, Point p) {
        if (this->crossProduct(e0->p1, e0->p2, p) < 0) {
            if (this->crossProduct(e1->p1, e1->p2, p) > 0) {
                return true;
            }
        }
        return false;
    }

    void VertexCalc::deleteTree(Node* node) {
        if (node->l != NULL) {
            delete node->l;
        }
        if (node->bn != NULL) {
            
            // this->deleteTree(node->bn->lst);
            // this->deleteTree(node->bn->rst);
            delete node->bn->e;
            delete node->bn;
        }
        if (node->t != NULL) {
            // this->deleteTree(node->t->lst);
            // this->deleteTree(node->t->mst);
            // this->deleteTree(node->t->rst);
            delete node->t->e1;
            delete node->t->e2;
            delete node->t->e3;
        }
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
