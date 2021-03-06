#pragma once

#include "core/app.h"
#include "render/window.h"
#include <vector>

#include "engine/node/Camera.h"
#include "engine/node/LightSource.h"
#include "engine/Scene.h"
#include "engine/io/Control.h"


namespace Graphics3D {
	class SimpleGraphics : public Core::App {
		public:

			SimpleGraphics();
			~SimpleGraphics();


			bool Open();
			void Run();


		private:

			glm::mat4 projection;

			Camera* camera;

			LightSource* lightSource;

			std::shared_ptr<ShaderProgram> shaderProgram;

			Display::Window* window;

			Control* control;

			float deltaTime;
			float lastFrame;

			GLint lightIntensityID;
			GLint lightColorID;
			GLint lightPosID;
			GLint cameraPosID;

			Scene* scene;

			const float pov = 45.0f;
            const float minViewDist = 0.1f;
            const float maxViewDist = 500.0f;

			uint frame;
			
	};
}
