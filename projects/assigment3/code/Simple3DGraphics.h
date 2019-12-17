#pragma once

#include "core/app.h"
#include "render/window.h"

#include "GraphicsNode.h"


namespace Simple3DGraphics {
	class Simple3DGraphics : public Core::App {
		public:

			Simple3DGraphics();
			~Simple3DGraphics();


			bool Open();
			void Run();


		private:

			GraphicsNode gNode;
		
	};
}
