#pragma once

#include <GL/glew.h>


namespace Graphics3D {
	class Reader {
		public:

            Reader();
            ~Reader();

            char* readFile(char* filePath);  

        private:

    };
} 
