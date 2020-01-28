#include "config.h"
#include "SimpleGraphics.h"
#include "engine/Mesh.h"
#include "engine/material/SimpleMaterial.h"
#include "engine/shader/ShaderProgram.h"
#include "engine/shader/Shader.h"
#include "engine/utils/ShaderType.h"
#include "engine/utils/Reader.h"
#include "engine/node/Camera.h"
#include "engine/utils/Colors.h"


#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


namespace Graphics3D {


    SimpleGraphics::SimpleGraphics() {
        this->deltaTime = 0;
        this->lastFrame = 0;
        this->frame = 0;
    }


    SimpleGraphics::~SimpleGraphics() {
        delete[] this->camera;
        delete[] this->lightSource;
        delete[] this->scene;
        delete[] this->control;
    }


    /**
     *  Initializes the program.
    */
    bool SimpleGraphics::Open() {
        App::Open();
        this->window = new Display::Window;

        if (this->window->Open()) {

            glEnable(GL_DEPTH_TEST); 

            // set clear color to gray
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            
            this->projection = glm::perspective(this->pov, (GLfloat) 200 / (GLfloat) 200, this->minViewDist, this->maxViewDist);

            this->camera = new Camera(this->window, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

            this->control = new Control(this->window, this->camera, this->camera);

            this->lightSource = new LightSource(glm::vec3(0, 10, 0), WHITE, 0.4f);


            Reader reader;
            GLchar* vst = (GLchar*) reader.readFile("/home/niklas/Desktop/D7045E/assigments/opengl-lab-env/projects/assigment4/code/engine/shader/shaders/SimpleShader.vert");
            GLchar* fst = (GLchar*) reader.readFile("/home/niklas/Desktop/D7045E/assigments/opengl-lab-env/projects/assigment4/code/engine/shader/shaders/SimpleShader.frag");

            std::shared_ptr<Shader> vShader(new Shader(vst, vertexShader));
            std::shared_ptr<Shader> fShader(new Shader(fst, fragmentShader));

            std::vector<std::shared_ptr<Shader>> shaders;
            shaders.push_back(vShader);
            shaders.push_back(fShader);

            std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram(shaders));
            this->shaderProgram = shaderProgram;

            this->scene = Scene::genScene(shaderProgram, 50000, 0, pov, minViewDist, maxViewDist);
            
            return true;
        }

        return false;
    }


    /**
     *  Program loop.
    */
    void SimpleGraphics::Run() {
        while (this->window->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		    this->window->Update();

            float currentFrame = glfwGetTime();
            this->deltaTime = currentFrame - this->lastFrame;
            this->lastFrame = currentFrame;

            this->control->update(this->deltaTime);

            this->shaderProgram->use();

            this->lightPosID = this->shaderProgram->getUniformId("lightPos");
            this->lightColorID = this->shaderProgram->getUniformId("lightColor");
            this->lightIntensityID = this->shaderProgram->getUniformId("lightIntensity");
            this->cameraPosID = this->shaderProgram->getUniformId("cameraPos");

            GLint projectionID = this->shaderProgram->getUniformId("projection");
            glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(this->projection));

            GLint cameraID = this->shaderProgram->getUniformId("camera");
            glUniformMatrix4fv(cameraID, 1, GL_FALSE, glm::value_ptr(this->camera->getCameraMatrix()));

            
            glm::vec3 lightPos = this->lightSource->getLightSourcePos();
            glUniform3f(this->lightPosID, lightPos[0], lightPos[1], lightPos[2]);
            glm::vec3 lightColor = this->lightSource->getColor();
            glUniform3f(this->lightColorID, lightColor[0], lightColor[1], lightColor[2]);
            glUniform1f(this->lightIntensityID, this->lightSource->getIntensity());

            glm::vec3 cameraPos = this->camera->getCameraPos();
            glUniform3f(this->cameraPosID, cameraPos[0], cameraPos[1], cameraPos[2]);


            glEnable(GL_CULL_FACE);  
            
            this->scene->renderScene(this->camera->getCameraDirection(), this->camera->getCameraPos(),
                this->deltaTime, this->frame);

            this->window->SwapBuffers();
            this->frame += 1;
        }
    }
}
