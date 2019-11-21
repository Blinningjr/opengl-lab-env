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


            // funcs
            void ReadPoints(std::string filePath);
			void GenRandomPoints(int numPoints);

			void calcConvexHull();
			
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