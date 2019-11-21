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

			void sort(GLfloat* points, int length);
			
			int getPointsLength();
			GLfloat* getPoints();

			int getConvexHullLength();
			GLfloat* getConvexHull();

		private:
            // Var
            Triangulation3d::Reader reader;

			int pointsLength;
			GLfloat* points;

            int convexHullLength;
			GLfloat* convexHull;

            // int triangulationLength;
			// GLfloat* triangulation;

            // Funcs
            

	};
} // namespace Triangulation3d