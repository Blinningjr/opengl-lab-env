#include "vertexcalc.h"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <algorithm>
#include <math.h>  

namespace Triangulation2d {

    /**
     *  Creates a reader with no read points 
     */
    VertexCalc::VertexCalc() {
        this->pointsLength = 0;
        this->points = std::shared_ptr<Point[]>(new Point[this->pointsLength]);
        this->convexHullLength = 0;
        this->convexHull = std::shared_ptr<Point[]>(new Point[this->convexHullLength]);
        this->triangulationLength = 0;
        this->triangulation = std::shared_ptr<Triangle[]>(new Triangle[this->triangulationLength]);
        
        this->pickedC.x = 0;
        this->pickedC.y = 0;
        this->pickedC.z = -1;

        this->pickedC.r = 0;
        this->pickedC.g = 0;
        this->pickedC.b = 1;
        this->pickedC.a = 1;
        std::shared_ptr<Node> n = std::shared_ptr<Node>(new Node());
        n->id = 0;
        this->nodeId = 1;
        this->leafId = 0;
        this->root = n;
        this->leafsLength = 0;
        this->leafs = std::shared_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[this->leafsLength]);

        this->pickCOption = 0;
    }


    /**
     *  Deletes all values made by this class. 
     */
    VertexCalc::~VertexCalc() {
    }


    /**
     *  Reads points form a file at the file path filePath. 
     */
    void VertexCalc::ReadPoints(std::string filePath) {
        this->reader.ReadPoints(filePath);
        int numCords = this->reader.getPointsLength()/2;
        GLfloat* tBuf = this->reader.getPoints();

        this->pointsLength = numCords;
        this->points = std::shared_ptr<Point[]>(new Point[this->pointsLength]);

        Point* tPs = new Point[numCords];

        for (int i = 0; i < numCords; i++) {
            tPs[i].x = tBuf[0 + i * 2];
            tPs[i].y = tBuf[1 + i * 2];
            tPs[i].z = -1;

            tPs[i].r = 1;
            tPs[i].g = 1;
            tPs[i].b = 1;
            tPs[i].a = 1;
        }
        std::sort(tPs, tPs + numCords);
        for (int i = 0; i < numCords; i++) {
            this->points[i] = tPs[i];
        }

        delete[] tPs;

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

        this->pointsLength = numPoints;
        this->points = std::shared_ptr<Point[]>(new Point[this->pointsLength]);

        Point* tPs = new Point[numPoints];

        for (int i = 0; i < numPoints; i++) {
            tPs[i].x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.8f)) - 0.9f;
            tPs[i].y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.8f)) - 0.9f;
            tPs[i].z = -1;

            tPs[i].r = 1;
            tPs[i].g = 1;
            tPs[i].b = 1;
            tPs[i].a = 1;
        }
        std::sort(tPs, tPs + numPoints);
        for (int i = 0; i < numPoints; i++) {
            this->points[i] = tPs[i];
        }

        delete[] tPs;

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
     * https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
     */
    void VertexCalc::calcConvexHull() {
        this->convexHullLength = 0;

        if (this->pointsLength == 3) {
            this->convexHullLength = this->pointsLength;
            this->convexHull = std::shared_ptr<Point[]>(new Point[this->convexHullLength]);
            for (int i = 0; i <  this->pointsLength; i++) {
                this->convexHull[i] = this->points[i];
            }
            return;
        }

        int lLen = 0;
        Point* l = new Point[this->pointsLength];

        int uLen = 0;
        Point* u = new Point[this->pointsLength];

        for (int i = 0; i < this->pointsLength; i++) {
            while (lLen >= 2 && this->crossProduct(l[lLen-2], l[lLen-1], this->points[i]) <= 0) {
                lLen--;
            }
                while (uLen >= 2 && this->crossProduct(u[uLen-2], u[uLen-1], this->points[this->pointsLength - 1 - i]) <= 0) {
                uLen--;
            }
            l[lLen] = this->points[i];
            lLen += 1;
            u[uLen] = this->points[this->pointsLength - 1 - i];
            uLen += 1;
	    }

        this->convexHullLength = lLen + uLen - 2;
        this->convexHull = std::shared_ptr<Point[]>(new Point[this->convexHullLength]);
        
        for (int i = 0; i < lLen - 1; i++) {
            this->convexHull[i] = l[i];
        }
        for (int i = 0; i < uLen - 1 ; i++) {
            this->convexHull[i + lLen - 1] = u[i];
        }

        if (this->pickCOption == 2) {
            this->rotatingCalipers();
        }
        delete[] l;
        delete[] u;
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
        this->nodeId = 0;
        this->leafId = 0;

        this->calcConvexHull();
        if (this->pickCOption == 0) {
            this->pickRandomC();
        } else if (this->pickCOption == 1) {
            this->pickSquareMiddleC();
        }

        this->pickedC.r = 0;
        this->pickedC.g = 0;
        this->pickedC.b = 1;
        this->pickedC.a = 1;

        int cLength = this->convexHullLength;
        // if (this->pickedCOnHull()) {
        //     cLength -= 1;
        // }
        this->triangulationLength = cLength;
        this->triangulation = this->calcTriangles(this->convexHull, this->convexHullLength, this->pickedC);

        this->root = this->createTree(this->convexHull, this->convexHullLength, NULL, NULL);

        int pos = 0;
        int restLength = this->pointsLength - this->convexHullLength - 1;
        if (restLength < 0) {
            restLength = 0;
        }
        Point rest[restLength];

        for (int i = 0; i < this->pointsLength; i++) {
            bool add = true;
            for (int j = 0; j < this->convexHullLength; j++) {
                if (this->convexHull[j] == this->points[i]) {
                    add = false;
                    break;
                }
            }
            if (add && (this->points[i] != this->pickedC)) {
                rest[pos] = this->points[i];
                pos += 1;
            }
        }
        for (int i = 0; i < restLength; i++) {
            int r = rand() % pos;
            Point p = rest[r];
            rest[r] = rest[pos - 1];
            rest[pos - 1] = p;
            this->insertPoint(p, this->root);
            pos -= 1;
        }

        this->colorSameColor(0, 0.5f, 1, 1);

        std::cout << "\nlength =";
        std::cout << this->triangulationLength;
        // std::cout << "\n";
        // std::cout << "\nDebug Tree \n";
        // this->debugTree(this->tree);
        Point p;
        p.x = 0;
        p.y = 0;
        std::shared_ptr<std::shared_ptr<Node>[]> nodeArr = std::shared_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[2]);
        nodeArr[0] = NULL;
        nodeArr[1] = NULL;
        this->getLeaf(p, this->root, nodeArr);
        std::cout << "\nDebug Leafs \n";
        this->debugLeafs(nodeArr[0]->l);
    }


    /**
     *  Picks the vertex closses to the center of the box created by the largest and smalles x and y value.
     */
    void VertexCalc::pickSquareMiddleC() {
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
     *  Picks a random point that is not part of the convex hull as C.
    */
    void VertexCalc::pickRandomC() {
        int pos = 0;
        int restLength = this->pointsLength - this->convexHullLength;
        if (restLength < 0) {
            restLength = 0;
        }
        Point rest[restLength];

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
        this->pickedC = rest[rand()%restLength];

    }

    /**
     * Picks C py calculating the largest diameter and finding the point closest to its middle.
     * http://www-cgrl.cs.mcgill.ca/~godfried/research/calipers.html
     * https://es.wikipedia.org/wiki/M%C3%A9todo_del_Calibre_Giratorio
    */
    void VertexCalc::rotatingCalipers() {
        auto dist = [] (Point a, Point b)  {
            return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
        };

        int k = 1;
        while ( abs(this->crossProduct(this->convexHull[this->convexHullLength - 1], this->convexHull[0], this->convexHull[(k + 1) % this->convexHullLength])) > 
                abs(this->crossProduct(this->convexHull[this->convexHullLength - 1], this->convexHull[0], this->convexHull[k]))) {
            ++k;
        }

        Point p1 = this->convexHull[0];
        Point p2 = this->convexHull[k];

        //Generate all antipodal pairs in i,j
        for (int i = 0, j = k; i <= k && j < this->convexHullLength; ++i) {
            if (dist(this->convexHull[i], this->convexHull[j]) > dist(p1, p2)) {
                p1 = this->convexHull[i];
                p2 = this->convexHull[j];
            }

            //Advance point j while j is antipodal pair of i
            while (j < this->convexHullLength && 
                (abs(this->crossProduct(this->convexHull[i], this->convexHull[(i + 1) % this->convexHullLength], this->convexHull[(j + 1) % this->convexHullLength])) > 
                abs(this->crossProduct(this->convexHull[i], this->convexHull[(i + 1) % this->convexHullLength], this->convexHull[j])))) {

                if (dist(this->convexHull[i], this->convexHull[(j + 1) % this->convexHullLength]) > dist(p1, p2)) {
                    p1 = this->convexHull[i];
                    p2 = this->convexHull[(j + 1) % this->convexHullLength];
                }
                ++j;
            }
        }

        GLfloat cx = 0;
        GLfloat cy = 0;
        cx = p1.x + (p2.x - p1.x)/2;
        cy = p1.y + (p2.y - p1.y)/2;
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
    std::shared_ptr<VertexCalc::Triangle[]> VertexCalc::calcTriangles(std::shared_ptr<Point[]> ps, int length, Point v) {
        std::shared_ptr<Triangle[]> triangles = std::shared_ptr<Triangle[]>(new Triangle[length]);
        for (int i = 0; i < length - 1; i++) {
            triangles[i].p1 = ps[i];
            triangles[i].p2 = v;
            triangles[i].p3 = ps[i + 1];
        }

        triangles[length-1].p1 = ps[length-1];
        triangles[length-1].p2 = v;
        triangles[length-1].p3 = ps[0];

        // delete this->leafs;
        this->leafsLength = length;
        this->leafs = std::shared_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[this->leafsLength]);

        std::shared_ptr<Node> pl = std::shared_ptr<Node>(new Node());
        pl->id = this->nodeId;
        this->nodeId += 1;
        pl->l = std::shared_ptr<Leaf>(new Leaf());
        pl->l->id = this->leafId;
        this->leafId += 1;
        pl->l->triangle = triangles[0];
        this->leafs[0] = pl;
        for (int i = 1; i < length; i++) {
            std::shared_ptr<Node> l = std::shared_ptr<Node>(new Node());
            l->id = this->nodeId;
            this->nodeId += 1;
            l->l = std::shared_ptr<Leaf>(new Leaf());
            l->l->id = this->leafId;
            this->leafId += 1;
            l->l->triangle = triangles[i];
            pl->l->rl = l->l;
            l->l->ll = pl->l;
            pl = l;
            this->leafs[i] = l;
        }
        pl->l->rl = this->leafs[0]->l;
        this->leafs[0]->l->ll = pl->l;

        return triangles;
    }


    /**
     *  Creates the initial balanced tree with the convex hull and point c.
    */
    std::shared_ptr<VertexCalc::Node> VertexCalc::createTree(std::shared_ptr<Point[]> ps, int length, Point* p, std::shared_ptr<Node> bn) {
        std::shared_ptr<Edge> edge = std::shared_ptr<Edge>(new Edge());
        edge->p1 = this->pickedC;
        edge->p2 = ps[length/2];
        
        Point tps[length-1];
        int j = 0;
        for (int i = 0; i < length; i++) {
            if (i == length/2) {
                // continue;
            } else {
                tps[j] = ps[i];
                j += 1;
            }
        }

        int lpsLength = 0;
        std::shared_ptr<Point[]> lps = std::shared_ptr<Point[]>(new Point[length]);

        int rpsLength = 0;
        std::shared_ptr<Point[]> rps = std::shared_ptr<Point[]>(new Point[length]);

        for (int i = 0; i < length-1; i++) {
            if (this->crossProduct(edge->p1, edge->p2, tps[i]) < -this->calcEpsilon(edge->p1, tps[i])) {
                rps[rpsLength] = tps[i];
                rpsLength += 1;
            } else if (this->crossProduct(edge->p1, edge->p2, tps[i]) > this->calcEpsilon(edge->p1, tps[i])) {
                lps[lpsLength] = tps[i];
                lpsLength += 1;
            } else {
                rps[rpsLength] = tps[i];
                rpsLength += 1;

                lps[lpsLength] = tps[i];
                lpsLength += 1;

                p = &tps[i];
                bn = std::shared_ptr<Node>(new Node());
                bn->id = this->nodeId;
                this->nodeId += 1;
            }
        }

        if (p == &edge->p2) {
            if (!bn->bn) {
                bn->bn = std::shared_ptr<BNode>(new BNode());
                bn->bn->e = edge;
                if (lpsLength == 0) {
                    bn->bn->lst = this->findLeaf(edge, true);
                } else {
                    bn->bn->lst = createTree(lps, lpsLength, p, bn);
                }
                
                if (rpsLength == 0) {
                    bn->bn->rst = this->findLeaf(edge, false);
                } else {
                    bn->bn->rst = createTree(rps, rpsLength, p, bn);
                }
            }

            return bn;
        } else {
            std::shared_ptr<Node> node = std::shared_ptr<Node>(new Node());
            node->id = this->nodeId;
            this->nodeId += 1;

            std::shared_ptr<BNode> bNode = std::shared_ptr<BNode>(new BNode());
            bNode->e = edge;

            if (lpsLength == 0) {
                bNode->lst = this->findLeaf(edge, true);
            } else {
                bNode->lst = createTree(lps, lpsLength, p, bn);
            }
            
            if (rpsLength == 0) {
                bNode->rst = this->findLeaf(edge, false);
            } else {
                bNode->rst = createTree(rps, rpsLength, p, bn);
            }

            node->bn = bNode;

            return node;
        }
    }


    /**
     *  Finds a leaf with edge edge and in position left/rigth.
     *  Not only works when triangulation is a circle of triangles.
    */
    std::shared_ptr<VertexCalc::Node> VertexCalc::findLeaf(std::shared_ptr<Edge> edge, bool left) {
        for (int i = 0; i < this->leafsLength; i++) {
            if (left && this->leafs[i]->l->triangle.p1 == edge->p2) {
                return this->leafs[i];
            } else if (!left && this->leafs[i]->l->triangle.p3 == edge->p2) {
                return this->leafs[i];
            }
        }
        std::cout << "error findLeaf \n";
        return NULL;
    }


    /**
     *  Finds which triangle p is in and triangulates it.
    */
    void VertexCalc::insertPoint(Point p, std::shared_ptr<Node> node0) {
        std::shared_ptr<std::shared_ptr<Node>[]> nodeArr = std::shared_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[2]);
        nodeArr[0] = NULL;
        nodeArr[1] = NULL;
        int numLeafs = this->getLeaf(p, node0, nodeArr);
        if (numLeafs == 1) {
            // std::cout << "insertPoint numleafs=1 \n";
            std::shared_ptr<Leaf> l = nodeArr[0]->l;
            // if (l == this->leaf) {
            //     this->leaf = l->ll;
            // }
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

            
            std::shared_ptr<Trenary> trenary = std::shared_ptr<Trenary>(new Trenary());
            trenary->v = p;
            trenary->e1 = std::shared_ptr<Edge>(new Edge());
            trenary->e1->p1 = p;
            trenary->e1->p2 = l->triangle.p1;

            trenary->e2 = std::shared_ptr<Edge>(new Edge());
            trenary->e2->p1 = p;
            trenary->e2->p2 = l->triangle.p2;

            trenary->e3 = std::shared_ptr<Edge>(new Edge());
            trenary->e3->p1 = p;
            trenary->e3->p2 = l->triangle.p3;

            trenary->lst = std::shared_ptr<Node>(new Node());
            trenary->lst->id = this->nodeId;
            this->nodeId += 1;
            trenary->lst->l = std::shared_ptr<Leaf>(new Leaf());
            trenary->lst->l->id = this->leafId;
            this->leafId += 1;
            trenary->lst->l->triangle = t1;

            trenary->mst = std::shared_ptr<Node>(new Node());
            trenary->mst->id = this->nodeId;
            this->nodeId += 1;
            trenary->mst->l = std::shared_ptr<Leaf>(new Leaf());
            trenary->mst->l->id = this->leafId;
            this->leafId += 1;
            trenary->mst->l->triangle = t2;

            trenary->rst = std::shared_ptr<Node>(new Node());
            trenary->rst->id = this->nodeId;
            this->nodeId += 1;
            trenary->rst->l = std::shared_ptr<Leaf>(new Leaf());
            trenary->rst->l->id = this->leafId;
            this->leafId += 1;
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
            std::shared_ptr<Triangle[]> triangle = std::shared_ptr<Triangle[]>(new Triangle[length]);
            for (int i = 0; i < this->triangulationLength; i++) {
                if (!(l->triangle == this->triangulation[i])) {
                    triangle[pos] = this->triangulation[i];
                    pos += 1;
                }
            }
            triangle[pos + 0] = t1;
            triangle[pos + 1] = t2;
            triangle[pos + 2] = t3;

            this->triangulationLength = length;
            this->triangulation = triangle;
            
        } else if (numLeafs == 2) {
            // std::cout << "insertPoint numleafs=2 \n";
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
            std::shared_ptr<Leaf> leaf1 = nodeArr[0]->l;
            std::shared_ptr<Leaf> leaf2 = nodeArr[1]->l;
            nodeArr[0]->l = NULL;
            nodeArr[1]->l = NULL;
            
            int dirNode1 = this->findDiractionOfNeighbor(leaf1, leaf2);
            int dirNode2 = this->findDiractionOfNeighbor(leaf2, leaf1);

            std::shared_ptr<Leaf> l1 = std::shared_ptr<Leaf>(new Leaf());
            l1->id = this->leafId;
            this->leafId += 1;
            l1->triangle.p2 = p;

            std::shared_ptr<Leaf> l2 = std::shared_ptr<Leaf>(new Leaf());
            l2->id = this->leafId;
            this->leafId += 1;
            l2->triangle.p2 = p;
            
            std::shared_ptr<Edge> edge1 = std::shared_ptr<Edge>(new Edge());
            edge1->p1 = p;

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


            std::shared_ptr<Leaf> l3 = std::shared_ptr<Leaf>(new Leaf());
            l3->id = this->leafId;
            this->leafId += 1;
            l3->triangle.p2 = p;

            std::shared_ptr<Leaf> l4 = std::shared_ptr<Leaf>(new Leaf());
            l4->id = this->leafId;
            this->leafId += 1;
            l4->triangle.p2 = p;
            
            std::shared_ptr<Edge> edge2 = std::shared_ptr<Edge>(new Edge());
            edge2->p1 = p;

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
                edge2->p2 = leaf2->triangle.p2;

                l3->triangle.p1 = leaf2->triangle.p3;
                l3->triangle.p3 = leaf2->triangle.p2;
                l3->ml = leaf2->rl;
                this->insertLeafPointer(leaf2->rl, leaf2, l3);

                l4->triangle.p1 = leaf2->triangle.p2;
                l4->triangle.p3 = leaf2->triangle.p1;
                l4->ml = leaf2->ll;
                this->insertLeafPointer(leaf2->ll, leaf2, l4);

            } else if (dirNode2 == 3) {
                edge2->p2 = leaf2->triangle.p1;

                l3->triangle.p1 = leaf2->triangle.p2;
                l3->triangle.p3 = leaf2->triangle.p1;
                l3->ml = leaf2->ll;
                this->insertLeafPointer(leaf2->ll, leaf2, l3);

                l4->triangle.p1 = leaf2->triangle.p1;
                l4->triangle.p3 = leaf2->triangle.p3;
                l4->ml = leaf2->ml;
                this->insertLeafPointer(leaf2->ml, leaf2, l4);

            } else {
                std::cout << "Error bnode 2 insertPoint \n";
            }

            l1->ll = l4;
            l1->rl = l2;

            l2->ll = l1;
            l2->rl = l3;

            l3->ll = l2;
            l3->rl = l4;

            l4->ll = l3;
            l4->rl = l1;

            std::shared_ptr<BNode> bNode1 = std::shared_ptr<BNode>(new BNode());
            bNode1->e = edge1;
            bNode1->lst = std::shared_ptr<Node>(new Node());
            bNode1->lst->id = this->nodeId;
            this->nodeId += 1;
            bNode1->lst->l = l2;
            bNode1->rst = std::shared_ptr<Node>(new Node());
            bNode1->rst->id = this->nodeId;
            this->nodeId += 1;
            bNode1->rst->l = l1;

            nodeArr[0]->bn = bNode1;
            
            std::shared_ptr<BNode> bNode2 = std::shared_ptr<BNode>(new BNode());
            bNode2->e = edge2;
            bNode2->lst = std::shared_ptr<Node>(new Node());
            bNode2->lst->id = this->nodeId;
            this->nodeId += 1;
            bNode2->lst->l = l4;
            bNode2->rst = std::shared_ptr<Node>(new Node());
            bNode2->rst->id = this->nodeId;
            this->nodeId += 1;
            bNode2->rst->l = l3;

            nodeArr[1]->bn = bNode2;


            int pos = 0;
            int length = this->triangulationLength + 2;
            std::shared_ptr<Triangle[]> triangle = std::shared_ptr<Triangle[]>(new Triangle[length]);
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

            this->triangulationLength = length;
            this->triangulation = triangle;
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
    int VertexCalc::getLeaf(Point p, std::shared_ptr<Node> node, std::shared_ptr<std::shared_ptr<Node>[]> found) {
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
            // std::cout << "Error leaf already ocupied\n";
            return 0;

        } else if (node->bn) {
            if (this->crossProduct(node->bn->e->p1, node->bn->e->p2, p) < -this->calcEpsilon(node->bn->e->p1, p)) {
                // std::cout << "bn right \n";
                // std::cout << node->bn->e->p1.x;
                // std::cout << " ";
                // std::cout << node->bn->e->p1.y;
                // std::cout << "\n";
                // std::cout << node->bn->e->p2.x;
                // std::cout << " ";
                // std::cout << node->bn->e->p2.y;
                // std::cout << "\n";
                return this->getLeaf(p, node->bn->rst, found);

            } else if (this->crossProduct(node->bn->e->p1, node->bn->e->p2, p) > this->calcEpsilon(node->bn->e->p1, p)) {
                // std::cout << "bn left \n";
                return this->getLeaf(p, node->bn->lst, found);

            } else if (this->onLineSeg(node->bn->e, &p)) {

                // std::cout << "bn both \n";
                this->getLeaf(p, node->bn->lst, found);
                this->getLeaf(p, node->bn->rst, found);
                return 2;

            } else {
                // std::cout << "Not on lineseg\n";
                return this->getLeaf(p, node->bn->rst, found);
            }
        } else if (node->t) {

            std::shared_ptr<Trenary> t = node->t;

            if (this->isInsideEdges(t->e1, t->e2, p)) {

                // std::cout << "t left \n";
                return this->getLeaf(p, t->lst, found);

            } else if (this->isInsideEdges(t->e3, t->e1, p)) {

                // std::cout << "t middle \n";
                return this->getLeaf(p, t->mst, found);

            } else if (this->isInsideEdges(t->e2, t->e3, p)) {

                // std::cout << "t right \n";
                return this->getLeaf(p, t->rst, found);

            } else if (abs(this->crossProduct(t->e1->p1, t->e1->p2, p)) <= this->calcEpsilon(t->e1->p1, p)) {
                
                if (this->onLineSeg(t->e1, &p)) {
                    // std::cout << "t middle and left \n";
                    this->getLeaf(p, t->mst, found);
                    this->getLeaf(p, t->lst, found);
                    return 2;
                } else {
                    std::cout << "error 1 \n";
                    return this->getLeaf(p, t->rst, found);   
                }

            } else if (abs(this->crossProduct(t->e2->p1, t->e2->p2, p)) <= this->calcEpsilon(t->e2->p1, p)) {

                if (this->onLineSeg(t->e2, &p)) {
                    // std::cout << "t left and right \n";
                    this->getLeaf(p, t->lst, found);
                    this->getLeaf(p, t->rst, found);
                    return 2;
                } else {
                    std::cout << "error 2 \n";
                    return this->getLeaf(p, t->mst, found);
                }

            } else if (abs(this->crossProduct(t->e3->p1, t->e3->p2, p)) <= this->calcEpsilon(t->e3->p1, p)) {

                if (this->onLineSeg(t->e3, &p)) {
                    // std::cout << "t middle and right \n";
                    this->getLeaf(p, t->rst, found);
                    this->getLeaf(p, t->mst, found);
                    return 2;
                } else {
                    std::cout << "error 3 \n";
                    return this->getLeaf(p, t->lst, found);
                }

            } else {
                std::cout << "Error Trenary getLeaf \n";
            }
        } else {
            std::cout << "Error Empty Node \n";
        }
        return 0;
    }


    /**
     *  Finds which diraction l1 is pointing to l0 and switches that pointer to l2.
    */
    void VertexCalc::insertLeafPointer(std::shared_ptr<Leaf> l0, std::shared_ptr<Leaf> l1, std::shared_ptr<Leaf> l2) {
        if (!l0 || !l1 || !l2) {
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
    int VertexCalc::findDiractionOfNeighbor(std::shared_ptr<Leaf> l0, std::shared_ptr<Leaf> l1) {
        if (!l0 || !l1) {
            std::cout << "Error NULL findDiractionOfNeighbor \n";
            return 0;
        }

        if (l0->ll) {
            if (l0->ll == l1) {
                return 1;
            } 
        }

        if (l0->ml) {
            if (l0->ml == l1) {
                return 2;
            } 
        }
        if (l0->rl) {
            if (l0->rl == l1) {
                return 3;
            } 
        }

        std::cout << "Error findDiractionOfNeighbor \n";
        return 0;
    }


    /**
     *  Checks if the point is to the right of e0 and to the left of e1.
    */
    bool VertexCalc::isInsideEdges(std::shared_ptr<Edge> e0, std::shared_ptr<Edge> e1, Point p) {
        if (this->crossProduct(e0->p1, e0->p2, p) < -this->calcEpsilon(e0->p1, p)) {
            if (this->crossProduct(e1->p1, e1->p2, p) > this->calcEpsilon(e1->p1, p)) {
                return true;
            }
        }
        return false;
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
     *  Cheack is p ins ond edge.
    */
    bool VertexCalc::onLineSeg(std::shared_ptr<Edge> edge, Point* point) {
        if (abs(this->crossProduct(edge->p1, edge->p2, *point)) > this->calcEpsilon(edge->p1, *point)) {
            return false;
        }
        
        float dotproduct = (point->x - edge->p1.x) * (edge->p2.x - edge->p1.x) + (point->y - edge->p1.y)*(edge->p2.y - edge->p1.y);
        if (dotproduct < 0) {
            return false;
        }

        float squaredlengt = (edge->p2.x - edge->p1.x)*(edge->p2.x - edge->p1.x) + (edge->p2.y - edge->p1.y)*(edge->p2.y - edge->p1.y);
        if (dotproduct > squaredlengt) {
            return false;
        }
       
        return true;
    }


    /**
     *  Calculates the value that indicates if point is on line because floating point values inaccuracy.
    */
    float VertexCalc::calcEpsilon(Point p1, Point p2) {
        return (this->epsilon * sqrt(pow(p2.x - p1.x, 2.0f) + pow(p2.y - p1.y, 2.0f)))/2.0f;
    }


    /**
     *  Prints whole tree.
    */
    void VertexCalc::debugTree(std::shared_ptr<Node> node) {
        std::cout << "id: ";
        std::cout << node->id;
        std::cout << " type: ";
        if (node->l) {
            std::cout << "leaf";
            std::cout << " lid: ";
            std::cout << node->l->id;
            std::cout << "\n";
        } else if (node->bn) {
            std::cout << "BNode\n";
            this->debugTree(node->bn->lst);
            this->debugTree(node->bn->rst);
        } else if (node->t) {
            std::cout << "Trenary\n";
            this->debugTree(node->t->lst);
            this->debugTree(node->t->mst);
            this->debugTree(node->t->rst);
        } else {
            std::cout << "None\n";
        }
    }


    /**
     *  Prints topology of leafs.
    */
    void VertexCalc::debugLeafs(std::shared_ptr<Leaf> leaf) {
        if (!leaf) {
            return;
        } else {
            if (leaf->debugde) {
                return;
            } else {
                leaf->debugde = true;
                std::cout << "id: ";
                std::cout << leaf->id;
                std::cout << " left: ";
                if (leaf->ll) {
                    std::cout << leaf->ll->id;
                } else {
                    std::cout << "NULL";
                }
                std::cout << " middle: ";
                if (leaf->ml) {
                    std::cout << leaf->ml->id;
                } else {
                    std::cout << "NULL";
                }
                std::cout << " right: ";
                if (leaf->rl) {
                    std::cout << leaf->rl->id;
                } else {
                    std::cout << "NULL";
                }
                std::cout << "\n";
                this->debugLeafs(leaf->ll);
                this->debugLeafs(leaf->ml);
                this->debugLeafs(leaf->rl);
            }
        }
    }


    /**
     *  Colors all points in the same color.
    */
    void VertexCalc::colorSameColor(float r, float g, float b, float a) {
        for (int i = 0; i < this->triangulationLength; i++) {
            this->triangulation[i].p1.r = r;
            this->triangulation[i].p1.g = g;
            this->triangulation[i].p1.b = b;
            this->triangulation[i].p1.a = a;

            this->triangulation[i].p2.r = r;
            this->triangulation[i].p2.g = g;
            this->triangulation[i].p2.b = b;
            this->triangulation[i].p2.a = a;

            this->triangulation[i].p3.r = r;
            this->triangulation[i].p3.g = g;
            this->triangulation[i].p3.b = b;
            this->triangulation[i].p3.a = a;
        }
    }


    /**
     *  Color all points by calculating the distance from point r, g and b. Which are three points picked from the convex hull. 
    */
    void VertexCalc::colorInterpolationColor() {
        int interval = this->convexHullLength/3;
        Point r = this->convexHull[0];
        Point g = this->convexHull[interval];
        Point b = this->convexHull[interval * 2];

        for (int i = 0; i < this->triangulationLength; i++) {
            this->triangulation[i].p1.r = this->calcColorForPoint(this->triangulation[i].p1.x - r.x, this->triangulation[i].p1.y - r.y);
            this->triangulation[i].p1.g = this->calcColorForPoint(this->triangulation[i].p1.x - g.x, this->triangulation[i].p1.y - g.y);
            this->triangulation[i].p1.b = this->calcColorForPoint(this->triangulation[i].p1.x - b.x, this->triangulation[i].p1.y - b.y);
            this->triangulation[i].p1.a = this->calcAlphaForPoint(r, g, b, this->triangulation[i].p1);

            this->triangulation[i].p2.r = this->calcColorForPoint(this->triangulation[i].p2.x - r.x, this->triangulation[i].p2.y - r.y);
            this->triangulation[i].p2.g = this->calcColorForPoint(this->triangulation[i].p2.x - g.x, this->triangulation[i].p2.y - g.y);
            this->triangulation[i].p2.b = this->calcColorForPoint(this->triangulation[i].p2.x - b.x, this->triangulation[i].p2.y - b.y);
            this->triangulation[i].p2.a = this->calcAlphaForPoint(r, g, b, this->triangulation[i].p2);

            this->triangulation[i].p3.r = this->calcColorForPoint(this->triangulation[i].p3.x - r.x, this->triangulation[i].p3.y - r.y);
            this->triangulation[i].p3.g = this->calcColorForPoint(this->triangulation[i].p3.x - g.x, this->triangulation[i].p3.y - g.y);
            this->triangulation[i].p3.b = this->calcColorForPoint(this->triangulation[i].p3.x - b.x, this->triangulation[i].p3.y - b.y);
            this->triangulation[i].p3.a = this->calcAlphaForPoint(r, g, b, this->triangulation[i].p3);
        }

    }


    /**
     *  Colors all triangles with the four-color therom.
    */
    void VertexCalc::fourColor() {
        Point p;
        p.x = 0;
        p.y = 0;
        std::shared_ptr<std::shared_ptr<Node>[]> nodeArr = std::shared_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[2]);
        nodeArr[0] = NULL;
        nodeArr[1] = NULL;
        this->getLeaf(p, this->root, nodeArr);
        std::shared_ptr<std::shared_ptr<Leaf>[]> leafs = std::shared_ptr<std::shared_ptr<Leaf>[]>(new std::shared_ptr<Leaf>[this->triangulationLength]);
        leafs[0] = nodeArr[0]->l;
        this->fourColorHelper(leafs, 0, 1, 0);
    }


    /**
     *  Helper function for recursively calculeting four-color for all triangles.
    */
    void VertexCalc::fourColorHelper(std::shared_ptr<std::shared_ptr<Leaf>[]> leafs, int start, int end, int pos) {
        if (leafs[start]) {
                float r = 0;
                float g = 0;
                float b = 0;
                float a = 1;

                bool colors[5];
                
                std::shared_ptr<bool[]> colorsl = getUsedColors(leafs[start]);
                for (int i = 0; i < 5; i++) {
                    colors[i] = colorsl[i];
                }

                if (colors[1]) {
                    leafs[start]->color = 1;
                    r = 1;
                    g = 0;
                    b = 0;
                } else if (colors[2]) {
                    leafs[start]->color = 2;
                    r = 0;
                    g = 1;
                    b = 0;
                } else if (colors[3]) {
                    leafs[start]->color = 3;
                    r = 0;
                    g = 0;
                    b = 1;
                } else if (colors[4]) {
                    leafs[start]->color = 4;
                    r = 0.5;
                    g = 0.5;
                    b = 0.5;
                } else {
                    leafs[start]->color = 1;
                    std::cout << "Error all colors are taken\n";
                }

                leafs[start]->triangle.p1.r = r;
                leafs[start]->triangle.p1.g = g;
                leafs[start]->triangle.p1.b = b;
                leafs[start]->triangle.p1.a = a;

                leafs[start]->triangle.p2.r = r;
                leafs[start]->triangle.p2.g = g;
                leafs[start]->triangle.p2.b = b;
                leafs[start]->triangle.p2.a = a;

                leafs[start]->triangle.p3.r = r;
                leafs[start]->triangle.p3.g = g;
                leafs[start]->triangle.p3.b = b;
                leafs[start]->triangle.p3.a = a;

                if (pos >= this->triangulationLength) {
                    std::cout << "Error pos >= triangulationLength\n";
                }
                this->triangulation[pos] = leafs[start]->triangle;
                pos += 1;

                if (leafs[start]->ll) {
                    if (leafs[start]->ll->color == 0) {
                        leafs[start]->ll->color = 5;
                        leafs[end] = leafs[start]->ll;
                        end += 1;
                    }
                }
                if (leafs[start]->ml) {
                    if (leafs[start]->ml->color == 0) {
                        leafs[start]->ml->color = 5;
                        leafs[end] = leafs[start]->ml;
                        end += 1;
                    }
                }
                if (leafs[start]->rl) {
                    if (leafs[start]->rl->color == 0) {
                        leafs[start]->rl->color = 5;
                        leafs[end] = leafs[start]->rl;
                        end += 1;
                    }
                }
                
            
        }
        start += 1;
        if (start < end) {
            this->fourColorHelper(leafs, start, end, pos);
        }
        if (leafs[start - 1]) {
            leafs[start - 1]->color = 0;
        }
    }


    /**
     *  Gets the color used by the leaf and its connected nodes.
    */
    std::shared_ptr<bool[]> VertexCalc::getUsedColors(std::shared_ptr<Leaf> leaf) {
        std::shared_ptr<bool[]> colors = std::shared_ptr<bool[]>(new bool[6]);
        for (int i = 0; i < 6; i++) {
            colors[i] = true;
        }
        if (leaf) {

            colors[leaf->color] = false;
            
            if (leaf->ll) {
                colors[leaf->ll->color] = false;
            }
            if (leaf->ml) {
                colors[leaf->ml->color] = false;
            }
            if (leaf->rl) {
                colors[leaf->rl->color] = false;
            }
        }
        return colors;
    }


    /**
     *  Calculates 1 - hypotinusa. 
     */
    float VertexCalc::calcColorForPoint(float x, float y) {
        return 1.0f - abs(sqrt(pow(x, 2.0f) + pow(y, 2.0f)))/1.5f;
    }


    /**
     *  Calculates alpha for point p.
     *  The alpha is 0 if the point is outside the triangle r-b-g.
     */
    float VertexCalc::calcAlphaForPoint(Point r, Point b, Point g, Point p) {
        if (this->crossProduct(r, b, p) < 0) {
            return 0.0f;
        }
        if (this->crossProduct(b, g, p) < 0) {
            return 0.0f;
        }
        if (this->crossProduct(g, r, p) < 0) {
            return 0.0f;
        }
        return 1.0f;
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
    std::shared_ptr<VertexCalc::Point[]> VertexCalc::getPoints() {
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
	std::shared_ptr<VertexCalc::Point[]> VertexCalc::getConvexHull() {
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
	std::shared_ptr<VertexCalc::Triangle[]> VertexCalc::getTriangulation() {
        return this->triangulation;
    }


    /**
     *  Get the point picked as the center of the points 
     */
    VertexCalc::Point VertexCalc::getPickedC() {
        return this->pickedC;
    }


    /**
     *  Get an int that repressents the method of picking C.
    */
    int VertexCalc::getPickCOption() {
        return this->pickCOption;
    }


    /**
     *  Set an int that repressents the method of picking C.
    */
    void VertexCalc::setPickCOption(int option) {
        this->pickCOption = option;
    }
}
