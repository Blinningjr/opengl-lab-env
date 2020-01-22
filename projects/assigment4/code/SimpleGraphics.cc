#include "config.h"
#include "SimpleGraphics.h"
#include "Mesh.h"
#include "SimpleMaterial.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Reader.h"
#include "Camera.h"
#include "Cube.h"
#include "Colors.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


namespace Graphics3D {


    SimpleGraphics::SimpleGraphics() {
        this->deltaTime = 0;
        this->lastFrame = 0;
    }


    SimpleGraphics::~SimpleGraphics() {
        delete[] this->camera;
        delete[] this->lightSource;
    }


    bool SimpleGraphics::Open() {
        App::Open();
        this->window = new Display::Window;

        if (this->window->Open()) {

            glEnable(GL_DEPTH_TEST);  
            
            this->projection = glm::perspective(45.0f, (GLfloat) 200 / (GLfloat) 200, 0.1f, 10000.0f);

            this->camera = new Camera(this->window, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            this->lightSource = new LightSource(glm::vec3(0, 2, 0), WHITE, 0.5f);


            Reader reader;
            GLchar* vst = (GLchar*) reader.readFile("/home/niklas/Desktop/D7045E/assigments/opengl-lab-env/projects/assigment4/code/SimpleShader.vert");
            GLchar* fst = (GLchar*) reader.readFile("/home/niklas/Desktop/D7045E/assigments/opengl-lab-env/projects/assigment4/code/SimpleShader.frag");

            std::shared_ptr<Shader> vShader(new Shader(vst, vertexShader));
            std::shared_ptr<Shader> fShader(new Shader(fst, fragmentShader));

            std::vector<std::shared_ptr<Shader>> shaders;
            shaders.push_back(vShader);
            shaders.push_back(fShader);

            std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram(shaders));
            this->shaderProgram = shaderProgram;

            this->lightPosID = this->shaderProgram->getUniformId("lightPos");
            this->lightColorID = this->shaderProgram->getUniformId("lightColor");
            this->lightIntensityID = this->shaderProgram->getUniformId("lightIntensity");
            this->cameraPosID = this->shaderProgram->getUniformId("cameraPos");


            // set clear color to gray
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            
            std::shared_ptr<SimpleMaterial> simpleMaterialRed(new SimpleMaterial(this->shaderProgram, RED));  
            this->gNodes.push_back(Cube(0.1, 0.1, 0.1, simpleMaterialRed, glm::vec3(0, 0, 0)));

            std::shared_ptr<SimpleMaterial> simpleMaterialGreen(new SimpleMaterial(this->shaderProgram, GREEN));  
            this->gNodes.push_back(Cube(10, 0.1, 10, simpleMaterialGreen, glm::vec3(0, -0.2, 0)));

            std::shared_ptr<SimpleMaterial> simpleMaterialBlue(new SimpleMaterial(this->shaderProgram, BLUE));  
            this->gNodes.push_back(Cube(0.1, 0.2, 0.02, simpleMaterialBlue, glm::vec3(0, 0, 0)));


            return true;
        }

        return false;
    }


    void SimpleGraphics::Run() {
        while (this->window->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		    this->window->Update();

            float currentFrame = glfwGetTime();
            this->deltaTime = currentFrame - this->lastFrame;
            this->camera->setDeltaTime(this->deltaTime);
            this->lastFrame = currentFrame;

            this->shaderProgram->use();

            GLint projectionID = this->shaderProgram->getUniformId("projection");
            glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(this->projection));

            GLint cameraID = this->shaderProgram->getUniformId("camera");
            glUniformMatrix4fv(cameraID, 1, GL_FALSE, glm::value_ptr(this->camera->getCameraMatrix()));

            
            glm::vec3 lightPos = this->lightSource->getLightSourcePos();
            glUniform3f(this->lightPosID, lightPos[0], lightPos[1], lightPos[2]);
            glm::vec3 lightColor = this->lightSource->getColor();
            glUniform3f(this->lightColorID, lightColor[0], lightColor[1], lightColor[2]);
            glUniform1f(this->lightIntensityID, this->lightSource->getIntensity());


            glEnable(GL_CULL_FACE);  

            for(int i = 0; i < this->gNodes.size(); i++) {
                this->gNodes[i].draw();
            }



            this->window->SwapBuffers();
        }
    }
}
