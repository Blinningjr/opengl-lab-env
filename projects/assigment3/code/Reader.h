#include <GL/glew.h>


namespace Simple3DGraphics {
	class Reader {
		public:

            Reader();
            ~Reader();

            char* readFile(char* filePath);  

    };
} 
