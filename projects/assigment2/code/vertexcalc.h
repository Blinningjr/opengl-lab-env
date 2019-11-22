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
			struct Point {
				GLfloat x, y, z, r, g, b, a;

				bool operator <(const Point &p) const {
					return x < p.x || (x == p.x && y < p.y);
				}
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

		private:
            // Var
            Triangulation3d::Reader reader;

			int pointsLength;
			VertexCalc::Point* points;

            int convexHullLength;
			VertexCalc::Point* convexHull;

            int triangulationLength;
			VertexCalc::Point* triangulation;

            // Funcs
			GLfloat crossProduct(VertexCalc::Point a, VertexCalc::Point b, VertexCalc::Point c);
            

	};
} // namespace Triangulation3d