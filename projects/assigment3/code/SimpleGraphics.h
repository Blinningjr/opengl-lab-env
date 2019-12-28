#pragma once

#include "core/app.h"
#include "render/window.h"
#include "Camera.h"
#include <vector>

#include "GraphicsNode.h"


namespace Simple3DGraphics {
	class SimpleGraphics : public Core::App {
		public:

			SimpleGraphics();
			~SimpleGraphics();


			bool Open();
			void Run();


		private:

			std::vector<GraphicsNode> gNodes;

			Camera* camera;

			std::shared_ptr<ShaderProgram> shaderProgram;

			Display::Window* window;



			void createGNode(std::vector<Vertex> vertices,  std::vector<unsigned int> indices, GLfloat color[3]);

			void createSquare(GLfloat color[3]);
		
	};
}
