#pragma once


#include "render/window.h"
#include "IWASD.h"


namespace Graphics3D {
	class Control {
		public:

            Control(Display::Window* window, IWASD* iwasd);
            ~Control();

            void update(float deltaTime);

        private:

            Display::Window* window;

            IWASD* iwasd;

    };
} 
