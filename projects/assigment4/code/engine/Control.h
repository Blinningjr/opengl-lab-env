#pragma once


#include "render/window.h"
#include "IWASD.h"
#include "IMouse.h"


namespace Graphics3D {
	class Control {
		public:

            Control(Display::Window* window, IWASD* wasd, IMouse* mouse);
            ~Control();

            void update(float deltaTime);

        private:

            Display::Window* window;

            IWASD* wasd;
            IMouse* mouse;

    };
} 
