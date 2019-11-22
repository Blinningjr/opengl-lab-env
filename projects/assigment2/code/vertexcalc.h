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
			};
			struct Edge {
				Point p1, p2;
			};
			struct Triangle {
				Point p1, p2, p3;
			};

			struct Node;
			struct Leaf {
				int triangle;
			};
			struct BNode {
				Edge* e;
				Node *st1, *st2;
			};
			struct Trenary {
				Point v;
				Edge *e1, *e2,* e3;
				Node *st1, *st2, *st3;
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
			Point* getTriangulation();

			Point getPickedC();

		private:
            // Var
            Triangulation3d::Reader reader;

			int pointsLength;
			Point* points;

            int convexHullLength;
			Point* convexHull;

            int triangulationLength;
			Point* triangulation;

			Point pickedC;

			Node* tree;

            // Funcs
			GLfloat crossProduct(Point a, Point b, Point c);

			void pickC(); 

			Node* createTree(Point points[], int length);
            

	};
} // namespace Triangulation3d