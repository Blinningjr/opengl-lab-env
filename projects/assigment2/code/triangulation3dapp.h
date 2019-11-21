#pragma once

#include "core/app.h"
#include "render/window.h"
#include "vertexcalc.h"

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

			GLint vsBufferLength;
			GLchar *vsBuffer;
			GLint fsBufferLength;
			GLchar *fsBuffer;
			// std::string compilerLog;

			int bufLength;
            GLfloat* buf;

			Triangulation3d::VertexCalc vertexcalc;

			// Funcs
			void UpdateVBO();

			void copyToVBO(GLfloat* points, int length);
			
			void RenderUI();

			void ReaderUI(bool* open);
			void GenRandPointsUI(bool* open);

	};
} // namespace Triangulation3d