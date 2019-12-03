#pragma once

#include "core/app.h"
#include "render/window.h"
#include "vertexcalc.h"
#include <memory>

namespace Triangulation2d {
	class Triangulation2dApp : public Core::App {
		public:
			// Var


			// constructor
			Triangulation2dApp();
			
            // destructor
			~Triangulation2dApp();


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
            std::unique_ptr<GLfloat[]>  bufVBO;

			Triangulation2d::VertexCalc vertexcalc;

			bool showPoints;
			bool showConvexHull;
			bool showC;
			bool showTriangulation;
			bool showTriangulationOutline;

			GLfloat angle;
			GLfloat dist;


			// Funcs
			void UpdateVBO();
			
			void RenderUI();

			void ReaderUI(bool* open);
			void GenRandPointsUI(bool* open);

	};
} // namespace Triangulation2d