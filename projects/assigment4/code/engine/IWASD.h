#pragma once


namespace Graphics3D {
	class IWASD {
		public:

            virtual void updateMove(float deltaTime) = 0;

            virtual void setWPressed(bool isWPressed) = 0;
            virtual void setAPressed(bool isAPressed) = 0;
            virtual void setSPressed(bool isSPressed) = 0;
            virtual void setDPressed(bool isDPressed) = 0;

            virtual void toggleFly() = 0;

    };
}
