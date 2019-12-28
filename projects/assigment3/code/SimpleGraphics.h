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
		
	};
}
