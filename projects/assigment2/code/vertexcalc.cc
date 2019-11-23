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
        if (this->points) {
            delete[] this->points;
        }
        if (this->convexHull) {
            delete[] this->convexHull;
        }
        this->deleteTree(this->tree);
        if (this->triangulation) {
            delete[] this->triangulation;
        }
    }


    /**
     *  Reads points form a file at the file path filePath. 
     */
    void VertexCalc::ReadPoints(std::string filePath) {
        this->reader.ReadPoints(filePath);
        int numCords = this->reader.getPointsLength()/2;
        GLfloat* tBuf = this->reader.getPoints();

        if (this->points) {
            delete[] this->points;
        }
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

        if (this->points) {
            delete[] this->points;
        }

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
        if (this->convexHull) {
            delete[] this->convexHull;
        }
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


    /**
     *  Calculates the triangulation of all points.
    */
    void VertexCalc::calcTriangulation() {
        this->calcConvexHull();
        this->pickC();

        if (this->triangulation) {
            delete[] this->triangulation;
        }
        int cLength = this->convexHullLength;
        if (this->pickedCOnHull()) {
            cLength -= 1;
        }
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
     *  Calculates the initial triangles where only the convex hull and point C.
    */
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


    /**
     *  Creates the initial balanced tree with the convex hull and point c.
    */
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

        if (lps) {
            delete[] lps;
        }
        if (rps) {
            delete[] rps;
        }
        
        
        return node;
    }


    /**
     *  Finds a leaf with edge edge and in position left/rigth.
     *  Not only works when triangulation is a circle of triangles.
    */
    VertexCalc::Node* VertexCalc::findLeaf(Edge* edge, bool left) {
        Node* node = new Node();

        Leaf* l = this->leaf;
        for (int i = 0; i < this->triangulationLength; i++) {
            if (left && l->triangle.p1 == edge->p2) {
                node->l = l;
                return node;
            } else if (!left && l->triangle.p3 == edge->p2) {
                node->l = l;
                return node;
            }
            l = l->ll;
        }
        std::cout << "error findLeaf \n";
        return NULL;
    }


    /**
     *  Finds which triangle p is in and triangulates it.
    */
    void VertexCalc::insertPoint(Point p, Node* node0) {
        Node* nodeArr[2];
        nodeArr[0] = NULL;
        nodeArr[1] = NULL;
        int numLeafs = this->getLeaf(p, node0, nodeArr, true);
        if (numLeafs == 1) {
            
            Leaf* l = nodeArr[0]->l;
            if (l == this->leaf) {
                this->leaf = l->ll;
            }
            nodeArr[0]->l = NULL;
   
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

            nodeArr[0]->t = trenary;


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

            if (this->triangulation) {
                delete[] this->triangulation;
            }
            this->triangulationLength = length;
            this->triangulation = triangle;


            if (l) {
                delete l;
            }
            
        } else if (numLeafs == 2) {
            std::cout << "insertPoint numleafs=2 \n";
            if (!nodeArr[0]) {
                std::cout << "Error nodeArr[0] = null insertPoint \n";
            }
            if (!nodeArr[1]) {
                std::cout << "Error nodeArr[1] = null insertPoint \n";
            }
            if (!nodeArr[0]->l) {
                std::cout << "Error leaf1 = null insertPoint \n";
            }
            if (!nodeArr[1]->l) {
                std::cout << "Error leaf2 = null insertPoint \n";
            }
            Leaf* leaf1 = nodeArr[0]->l;
            Leaf* leaf2 = nodeArr[1]->l;
            nodeArr[0]->l = NULL;
            nodeArr[1]->l = NULL;


            Leaf* l1 = new Leaf();
            l1->triangle.p2 = p;

            Leaf* l2 = new Leaf();
            l2->triangle.p2 = p;
            
            Edge* edge1 = new Edge();
            edge1->p1;

            int dirNode1 = this->findDiractionOfNeighbor(leaf1, leaf2);
            if (dirNode1 == 1) {
                edge1->p2 = leaf1->triangle.p3;

                l1->triangle.p1 = leaf1->triangle.p1;
                l1->triangle.p3 = leaf1->triangle.p3;
                l1->ml = leaf1->ml;
                this->insertLeafPointer(leaf1->ml, leaf1, l1);

                l2->triangle.p1 = leaf1->triangle.p3;
                l2->triangle.p3 = leaf1->triangle.p2;
                l2->ml = leaf1->rl;
                this->insertLeafPointer(leaf1->rl, leaf1, l2);

            } else if (dirNode1 == 2) {
                edge1->p2 = leaf1->triangle.p2;

                l1->triangle.p1 = leaf1->triangle.p3;
                l1->triangle.p3 = leaf1->triangle.p2;
                l1->ml = leaf1->rl;
                this->insertLeafPointer(leaf1->rl, leaf1, l1);

                l2->triangle.p1 = leaf1->triangle.p2;
                l2->triangle.p3 = leaf1->triangle.p1;
                l2->ml = leaf1->ll;
                this->insertLeafPointer(leaf1->ll, leaf1, l2);

            } else if (dirNode1 == 3) {
                edge1->p2 = leaf1->triangle.p1;

                l1->triangle.p1 = leaf1->triangle.p2;
                l1->triangle.p3 = leaf1->triangle.p1;
                l1->ml = leaf1->ll;
                this->insertLeafPointer(leaf1->ll, leaf1, l1);

                l2->triangle.p1 = leaf1->triangle.p1;
                l2->triangle.p3 = leaf1->triangle.p3;
                l2->ml = leaf1->ml;
                this->insertLeafPointer(leaf1->ml, leaf1, l2);

            } else {
                std::cout << "Error bnode 1 insertPoint \n";
            }


            Leaf* l3 = new Leaf();
            l3->triangle.p2 = p;

            Leaf* l4 = new Leaf();
            l4->triangle.p2 = p;
            
            Edge* edge2 = new Edge();
            edge2->p1 = p;
            
            int dirNode2 = this->findDiractionOfNeighbor(nodeArr[1]->l, nodeArr[0]->l);
            if (dirNode2 == 1) {
                edge2->p2 = leaf2->triangle.p3;

                l3->triangle.p1 = leaf2->triangle.p1;
                l3->triangle.p3 = leaf2->triangle.p3;
                l3->ml = leaf2->ml;
                this->insertLeafPointer(leaf2->ml, leaf2, l3);

                l4->triangle.p1 = leaf2->triangle.p3;
                l4->triangle.p3 = leaf2->triangle.p2;
                l4->ml = leaf2->rl;
                this->insertLeafPointer(leaf2->rl, leaf2, l4);

            } else if (dirNode2 == 2) {
                edge1->p2 = leaf2->triangle.p2;

                l3->triangle.p1 = leaf2->triangle.p3;
                l3->triangle.p3 = leaf2->triangle.p2;
                l3->ml = leaf2->rl;
                this->insertLeafPointer(leaf2->rl, leaf2, l3);

                l4->triangle.p1 = leaf2->triangle.p2;
                l4->triangle.p3 = leaf2->triangle.p1;
                l4->ml = leaf2->ll;
                this->insertLeafPointer(leaf2->ll, leaf2, l4);

            } else if (dirNode2 == 3) {
                edge1->p2 = leaf2->triangle.p1;

                l3->triangle.p1 = leaf2->triangle.p2;
                l3->triangle.p3 = leaf2->triangle.p1;
                l3->ml = leaf2->ll;
                this->insertLeafPointer(leaf2->ll, leaf2, l3);

                l4->triangle.p1 = leaf2->triangle.p1;
                l4->triangle.p3 = leaf2->triangle.p3;
                l4->ml = leaf2->ml;
                this->insertLeafPointer(leaf2->ml, leaf2, l4);

            } else {
                std::cout << "Error bnode 1 insertPoint \n";
            }

            l1->ll = l4;
            l1->rl = l2;

            l2->ll = l1;
            l2->rl = l3;

            l3->ll = l2;
            l3->rl = l4;

            l4->ll = l3;
            l4->rl = l1;

            BNode* bNode1 = new BNode();
            bNode1->e = edge1;
            bNode1->lst = new Node();
            bNode1->lst->l = l2;
            bNode1->rst = new Node();
            bNode1->rst->l = l1;

            nodeArr[0]->bn = bNode1;
            
            BNode* bNode2 = new BNode();
            bNode2->e = edge2;
            bNode2->lst = new Node();
            bNode2->lst->l = l4;
            bNode2->rst = new Node();
            bNode2->rst->l = l3;

            nodeArr[1]->bn = bNode2;


            int pos = 0;
            int length = this->triangulationLength + 2;
            Triangle* triangle = new Triangle[length];
            for (int i = 0; i < this->triangulationLength; i++) {
                if (!(leaf1->triangle == this->triangulation[i]) && !(leaf2->triangle == this->triangulation[i])) {
                    triangle[pos] = this->triangulation[i];
                    pos += 1;
                }
            }
            triangle[pos + 0] = l1->triangle;
            triangle[pos + 1] = l2->triangle;
            triangle[pos + 2] = l3->triangle;
            triangle[pos + 3] = l4->triangle;

            if (this->triangulation) {
                delete[] this->triangulation;
            }
            this->triangulationLength = length;
            this->triangulation = triangle;


            if (leaf1) {
                delete leaf1;
            }
            if (leaf2) {
                delete leaf2;
            }
        } else {
            std::cout << "Error insertPoint numleafs=";
            std::cout <<  numLeafs;
            std::cout << "\n";
        }
    }

    
    /**
     *  Finds what leaf p is in and return that node in found.
     *  Note: there can be two nodes found if p is on a edge. 
    */
    int VertexCalc::getLeaf(Point p, Node* node, Node* found[2], bool one) {
        one = true;
        if (node->l) {
            if (!found[0]) {
                found[0] = node;
                return 1;
            } else {
                if (!found[0]->l) {
                    found[0] = node;   
                    return 1;
                }
            }
            if (!found[1]) {
                found[1] = node;
                return 1;
            } else {
                if (!found[1]->l) {
                    found[1] = node;   
                    return 1;
                }
            }
            std::cout << "Error leaf already ocupied\n";
            return 0;

        } else if (node->bn) {
            if (this->crossProduct(node->bn->e->p1, node->bn->e->p2, p) < 0) {
          
                return this->getLeaf(p, node->bn->rst, found, one);

            } else if (this->crossProduct(node->bn->e->p1, node->bn->e->p2, p) > 0) {

                return this->getLeaf(p, node->bn->lst, found, one);

            } else if (one) {

                this->getLeaf(p, node->bn->lst, found, false);
                this->getLeaf(p, node->bn->rst, found, false);
                return 2;

            } else {

                std::cout << "Error BNode getLeaf \n";

            }
        } else if (node->t) {

            Trenary* t = node->t;

            if (this->isInsideEdges(t->e1, t->e2, p)) {

                return this->getLeaf(p, t->lst, found, one);

            } else if (this->isInsideEdges(t->e3, t->e1, p)) {

                return this->getLeaf(p, t->mst, found, one);

            } else if (this->isInsideEdges(t->e2, t->e3, p)) {

                return this->getLeaf(p, t->rst, found, one);

            } else if (one && this->crossProduct(t->e1->p1, t->e1->p2, p) == 0) {

                this->getLeaf(p, t->mst, found, false);
                this->getLeaf(p, t->lst, found, false);
                return 2;

            } else if (one && this->crossProduct(t->e2->p1, t->e2->p2, p) == 0) {

                this->getLeaf(p, t->lst, found, false);
                this->getLeaf(p, t->rst, found, false);
                return 2;

            } else if (one && this->crossProduct(t->e3->p1, t->e3->p2, p) == 0) {

                this->getLeaf(p, t->rst, found, false);
                this->getLeaf(p, t->mst, found, false);
                return 2;

            } else {
                
                std::cout << "Error Trenary getLeaf one =";
                std::cout << one;
                std::cout << "\n";
            }
        } else {
            std::cout << "Error Empty Node \n";
        }
        return 0;
    }


    /**
     *  Finds which diraction l1 is pointing to l0 and switches that pointer to l2.
    */
    void VertexCalc::insertLeafPointer(Leaf* l0, Leaf* l1, Leaf* l2) {
        if (!l0) {
            // std::cout << "NULL insertLeafPointer \n";
            return;
        }

        if (l0->ll) {
            if (l0->ll->triangle == l1->triangle) {
                l0->ll = l2;
                return;
            } 
        }

        if (l0->ml) {
            if (l0->ml->triangle == l1->triangle) {
                l0->ml = l2;
                return;
            } 
        }
        if (l0->rl) {
            if (l0->rl->triangle == l1->triangle) {
                l0->rl = l2;
                return;
            } 
        }

        std::cout << "Error insertLeafPointer \n";
    }


    /**
     *  Gets which direction l1 is to l0-
     *  0 = l0 == null, 1 = left, 2 = midel, 3 = right
    */
    int VertexCalc::findDiractionOfNeighbor(Leaf* l0, Leaf* l1) {
        if (!l0 || !l1) {
            return 0;
        }

        if (l0->ll) {
            if (l0->ll->triangle == l1->triangle) {
                return 1;
            } 
        }

        if (l0->ml) {
            if (l0->ml->triangle == l1->triangle) {
                return 2;
            } 
        }
        if (l0->rl) {
            if (l0->rl->triangle == l1->triangle) {
                return 3;
            } 
        }

        std::cout << "Error findDiractionOfNeighbor \n";
        return 0;
    }


    /**
     *  Checks if the point is to the right of e0 and to the left of e1.
    */
    bool VertexCalc::isInsideEdges(Edge* e0, Edge* e1, Point p) {
        if (this->crossProduct(e0->p1, e0->p2, p) < 0) {
            if (this->crossProduct(e1->p1, e1->p2, p) > 0) {
                return true;
            }
        }
        return false;
    }


    /**
     *  Delete all nodes in tree.
    */
    void VertexCalc::deleteTree(Node* node) {
        // if (node) {
        //     if (node->l) {
        //         delete node->l;
        //     }
        // }

        if (node) {
            if (node->bn) {
                if (node->bn->lst) {
                    this->deleteTree(node->bn->lst);
                }
            }
        }
        if (node) {
            if (node->bn) {
                if (node->bn->rst) {
                    this->deleteTree(node->bn->rst);
                }
            }
        }
        if (node) {
            if (node->bn) {
                if (node->bn->e) {
                    delete node->bn->e;
                }
            }
        }
        if (node) {
            if (node->bn) {
                delete node->bn;
            }
        }
        if (node) {
            if (node->t) {
                if (node->t->lst) {
                    this->deleteTree(node->t->lst);
                }
            }
        }
        if (node) {
            if (node->t) {
                if (node->t->mst) {
                    this->deleteTree(node->t->mst);
                }
            }
        }
        if (node) {
            if (node->t) {
                if (node->t->rst) {
                    this->deleteTree(node->t->rst);
                }
            }
        }
        if (node) {
            if (node->t) {
                if (node->t->e1) {
                    delete node->t->e1;
                }
            }
        }
        if (node) {
            if (node->t) {
                if (node->t->e2) {
                    delete node->t->e2;
                }
            }
        }
        if (node) {
            if (node->t) {
                if (node->t->e3) {
                    delete node->t->e3;
                }
            }
        }
        if (node) {
            delete node;
        }
    }


    /**
     *  Checks if picked C is on the convex hull.
    */
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
