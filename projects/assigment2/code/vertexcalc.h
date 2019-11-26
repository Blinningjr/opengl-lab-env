#include <string>
#include <GL/glew.h>
#include "reader.h"
#include <memory>

namespace Triangulation3d {
	class VertexCalc {
		public:
			// constructor
			VertexCalc();
			
            // destructor
			~VertexCalc();
        
            // Var

			// structs
			struct Point {
				GLfloat x, y, z, r, g, b, a;

				bool operator <(const Point &p) const {
					return x < p.x || (x == p.x && y < p.y);
				}
				bool operator ==(const Point& p){
					return x == p.x && y == p.y;
				}
				bool operator !=(const Point& p){
					return x != p.x || y != p.y;
				}
			};
			struct Edge {
				Point p1, p2;
			};
			struct Triangle {
				Point p1, p2, p3;
				bool operator ==(const Triangle& t){
					return p1 == t.p1 && p2 == t.p2 && p3 == t.p3;
				}
			};

			struct Node;
			struct Leaf {
				int id;
				int color = 0;
				bool debugde = false;

				Triangle triangle;
				Leaf *ll, *ml, *rl;

				bool operator ==(const Leaf& l){
					return triangle == l.triangle;
				}
			};
			struct BNode {
				Edge* e;
				Node *lst, *rst;
			};
			struct Trenary {
				Point v;
				Edge *e1, *e2,* e3;
				Node *lst, *mst, *rst;
			};
			struct Node {
				int id;
				Leaf* l;
				BNode* bn;
				Trenary* t;
			};


            // funcs
            void ReadPoints(std::string filePath);
			void GenRandomPoints(int numPoints);

			void calcConvexHull();

			void calcTriangulation();
			
			int getPointsLength();
			Point* getPoints();

			int getConvexHullLength();
			std::shared_ptr<Point[]> getConvexHull();

			int getTriangulationLength();
			std::shared_ptr<Triangle[]> getTriangulation();

			Point getPickedC();

			void colorSameColor(float r, float g, float b, float a);
			void colorInterpolationColor();
			void fourColor();

		private:
            // Var
            Triangulation3d::Reader reader;

			int pointsLength;
			Point* points;

            int convexHullLength;
			std::shared_ptr<Point[]> convexHull;

            int triangulationLength;
			std::shared_ptr<Triangle[]> triangulation;

			Point pickedC;

			Node* tree;
			
			int leafsLength;
			Node** leafs;

			int nodeId;
			int leafId;

			float epsilon = 0.01f;

            // Funcs
			GLfloat crossProduct(Point a, Point b, Point c);

			void pickC(); 

			std::shared_ptr<Triangle[]> calcTriangles(std::shared_ptr<Point[]> ps, int length, Point v);

			Node* createTree(std::shared_ptr<Point[]> ps, int length, Point* p, Node* bn);

			Node* findLeaf(Edge* edge, bool left);

			void insertPoint(Point p, Node* node);

			int getLeaf(Point p, Node* node, Node* found[2]);

			void insertLeafPointer(Leaf* l0, Leaf* l1, Leaf* l2);

			int findDiractionOfNeighbor(Leaf* l0, Leaf* l1);

			bool isInsideEdges(Edge* e0, Edge* e1, Point p);

			void deleteTree(Node* node);

			bool pickedCOnHull();

			bool onLineSeg(Edge* edge, Point* point);

			float calcEpsilon(Point p1, Point p2);
            
			void debugTree(Node* node);

			void debugLeafs(Leaf* leaf);

			float calcColorForPoint(float x, float y);
			float calcAlphaForPoint(Point r, Point b, Point g, Point p);

			int fourColorHelper(Leaf* leaf, int pos, bool* colors);

	};
} // namespace Triangulation3d