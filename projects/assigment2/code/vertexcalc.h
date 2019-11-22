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
			enum NodeType {
				LEAF,
				BNODE,
				TRENARY,
			};
			struct Leaf {
				NodeType nType;
				void* parent;
				Triangle t;
			};
			struct BNode {
				NodeType nType;
				void* parent;
				Edge e;
				void* st1;
				void* st2;
			};
			struct Trenary {
				NodeType nType;
				void* parent;
				Point v;
				Edge e1, e2, e3;
				void* st1;
				void* st2;
				void* st3;
			};


            // funcs
            void ReadPoints(std::string filePath);
			void GenRandomPoints(int numPoints);

			void calcConvexHull();

			void calcTriangulation();
			
			int getPointsLength();
			VertexCalc::Point* getPoints();

			int getConvexHullLength();
			VertexCalc::Point* getConvexHull();

			int getTriangulationLength();
			VertexCalc::Point* getTriangulation();

			VertexCalc::Point getPickedC();

		private:
            // Var
            Triangulation3d::Reader reader;

			int pointsLength;
			VertexCalc::Point* points;

            int convexHullLength;
			VertexCalc::Point* convexHull;

            int triangulationLength;
			VertexCalc::Point* triangulation;

			VertexCalc::Point pickedC;

            // Funcs
			GLfloat crossProduct(VertexCalc::Point a, VertexCalc::Point b, VertexCalc::Point c);

			void pickC(); 
            

	};
} // namespace Triangulation3d