#pragma once


#include "config.h"


namespace Graphics3D {
	class IMouse {
		public:

            virtual void updateLook(float deltaTime) = 0;

            virtual void setMousePos(float64 xPos, float64 yPos) = 0;

    };
}
