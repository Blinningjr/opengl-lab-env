#pragma once

#include "core/app.h"
#include "render/window.h"
#include "reader.h"

namespace Triangulation3d {
	class Triangulation3dApp : public Core::App {
		public:
			// Var


			// constructor
			Triangulation3dApp();
			
            // destructor
			~Triangulation3dApp();


            // funcs
			bool Open();

			void Run();


		private:
			// Var
			GLuint program;
			GLuint vertexShader;
			GLuint pixelShader;
			GLuint triangle;
			Display::Window* window;

			int bufLength;
            GLfloat* buf;

			Triangulation3d::Reader reader;

			// Funcs
			void updateBuf();
			

	};
} // namespace Triangulation3d