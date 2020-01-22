#pragma once

#include "core/app.h"
#include "render/window.h"
#include <vector>

#include "GraphicsNode.h"
#include "Camera.h"
#include "LightSource.h"
#include "Scene.h"


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

			float deltaTime;
			float lastFrame;

			GLint lightIntensityID;
			GLint lightColorID;
			GLint lightPosID;
			GLint cameraPosID;

			Scene* scene;
			
	};
}
