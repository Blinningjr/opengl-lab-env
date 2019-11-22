#include <string>
#include <GL/glew.h>
#include "reader.h"

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
			};
			struct Edge {
				Point p1, p2;
			};
			struct Triangle {
				Point p1, p2, p3;
			};

			struct Node;
			struct Leaf {
				Triangle triangle;
				Leaf *ll, *rl;
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
			Point* getConvexHull();

			int getTriangulationLength();
			Triangle* getTriangulation();

			Point getPickedC();

		private:
            // Var
            Triangulation3d::Reader reader;

			int pointsLength;
			Point* points;

            int convexHullLength;
			Point* convexHull;

            int triangulationLength;
			Triangle* triangulation;

			Point pickedC;

			Node* tree;
			Leaf* leaf;

            // Funcs
			GLfloat crossProduct(Point a, Point b, Point c);

			void pickC(); 

			Triangle* calcTriangles(Point* points, int length, Point v);

			Node* createTree(Point* points, int length);

			Node* findTriangle(Edge* edge, bool left);

			void deleteTree(Node* node);

			bool pickedCOnHull();
            

	};
} // namespace Triangulation3d