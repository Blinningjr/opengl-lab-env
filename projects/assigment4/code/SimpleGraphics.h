#pragma once

#include "core/app.h"
#include "render/window.h"
#include <vector>

#include "GraphicsNode.h"
#include "Camera.h"
#include "LightSource.h"


namespace Graphics3D {
	class SimpleGraphics : public Core::App {
		public:

			SimpleGraphics();
			~SimpleGraphics();


			bool Open();
			void Run();


		private:

			std::vector<GraphicsNode> gNodes;

			glm::mat4 projection;

			Camera* camera;

			LightSource* lightSource;

			std::shared_ptr<ShaderProgram> shaderProgram;

			Display::Window* window;


			GLfloat xPos = 0;
			GLfloat yPos = 0;
			GLfloat zPos = -1;


			void createGNode(std::vector<Vertex> vertices,  std::vector<unsigned int> indices, glm::vec3 pos, GLfloat color[3]);

			void createBox(glm::vec3 pos, GLfloat color[3]);
		
	};
}
