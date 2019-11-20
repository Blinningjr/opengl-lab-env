#include <string>
#include <GL/glew.h>

namespace Triangulation3d {
	class Reader {
		public:
			// constructor
			Reader();
			
            // destructor
			~Reader();
        
            // Var


            // funcs
            void readPoints(std::string file_path);
			int getPointsLength();
			GLfloat* getPoints();

		private:
            // Var
			int pointsLength;
			GLfloat* points;

            // Funcs
            

	};
} // namespace Triangulation3d