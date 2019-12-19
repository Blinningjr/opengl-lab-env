#pragma once

#include "core/app.h"
#include "render/window.h"

#include "GraphicsNode.h"


namespace Simple3DGraphics {
	class SimpleGraphics : public Core::App {
		public:

			SimpleGraphics();
			~SimpleGraphics();


			bool Open();
			void Run();


		private:

			GraphicsNode* gNode;

			Display::Window* window;
		
	};
}
