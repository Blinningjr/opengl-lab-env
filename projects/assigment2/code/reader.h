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
            void ReadPoints(std::string filePath);
			
			int getPointsLength();
			GLfloat* getPoints();

			std::string getFilePath();

		private:
            // Var
			int pointsLength;
			GLfloat* points;

			std::string filePath;
            // Funcs
            

	};
} // namespace Triangulation3d