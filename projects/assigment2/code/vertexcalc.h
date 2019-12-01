#include <string>
#include <GL/glew.h>
#include "reader.h"
#include <memory>

namespace Triangulation2d {
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
				std::shared_ptr<Leaf> ll, ml, rl;

				bool operator ==(const Leaf& l){
					return triangle == l.triangle;
				}
			};
			struct BNode {
				std::shared_ptr<Edge> e;
				std::shared_ptr<Node> lst, rst;
			};
			struct Trenary {
				Point v;
				std::shared_ptr<Edge> e1, e2, e3;
				std::shared_ptr<Node> lst, mst, rst;
			};
			struct Node {
				int id;
				std::shared_ptr<Leaf> l;
				std::shared_ptr<BNode> bn;
				std::shared_ptr<Trenary> t;
			};


            // funcs
            void ReadPoints(std::string filePath);
			void GenRandomPoints(int numPoints);

			void calcConvexHull();

			void calcTriangulation();
			
			int getPointsLength();
			std::shared_ptr<Point[]> getPoints();

			int getConvexHullLength();
			std::shared_ptr<Point[]> getConvexHull();

			int getTriangulationLength();
			std::shared_ptr<Triangle[]> getTriangulation();

			Point getPickedC();

			void colorSameColor(float r, float g, float b, float a);
			void colorInterpolationColor();
			void fourColor();

			int getPickCOption();
			void setPickCOption(int option);

		private:
            // Var
            Triangulation2d::Reader reader;

			int pointsLength;
			std::shared_ptr<Point[]> points;

            int convexHullLength;
			std::shared_ptr<Point[]> convexHull;

            int triangulationLength;
			std::shared_ptr<Triangle[]> triangulation;

			Point pickedC;

			std::shared_ptr<Node> root;
			
			int leafsLength;
			std::shared_ptr<std::shared_ptr<Node>[]> leafs;

			int nodeId;
			int leafId;

			float epsilon = 0.01f;

			int pickCOption;

            // Funcs
			GLfloat crossProduct(Point a, Point b, Point c);

			void pickSquareMiddleC();

			void pickRandomC();

			void rotatingCalipers();

			std::shared_ptr<Triangle[]> calcTriangles(std::shared_ptr<Point[]> ps, int length, Point v);

			std::shared_ptr<Node> createTree(std::shared_ptr<Point[]> ps, int length, Point* p, std::shared_ptr<Node> bn);

			std::shared_ptr<Node> findLeaf(std::shared_ptr<Edge> edge, bool left);

			void insertPoint(Point p, std::shared_ptr<Node> node0);

			int getLeaf(Point p, std::shared_ptr<Node> node, std::shared_ptr<std::shared_ptr<Node>[]> found);

			void insertLeafPointer(std::shared_ptr<Leaf> l0, std::shared_ptr<Leaf> l1, std::shared_ptr<Leaf> l2);

			int findDiractionOfNeighbor(std::shared_ptr<Leaf> l0, std::shared_ptr<Leaf> l1);

			bool isInsideEdges(std::shared_ptr<Edge> e0, std::shared_ptr<Edge> e1, Point p);

			bool pickedCOnHull();

			bool onLineSeg(std::shared_ptr<Edge> edge, Point* point);

			float calcEpsilon(Point p1, Point p2);
            
			void debugTree(std::shared_ptr<Node> node);

			void debugLeafs(std::shared_ptr<Leaf> leaf);

			float calcColorForPoint(float x, float y);
			float calcAlphaForPoint(Point r, Point b, Point g, Point p);

			void fourColorHelper(std::shared_ptr<std::shared_ptr<Leaf>[]> leafs, int start, int end, int pos);
			
			std::shared_ptr<bool[]> getUsedColors(std::shared_ptr<Leaf> leaf);

	};
} // namespace Triangulation2d