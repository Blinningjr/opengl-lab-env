#include "config.h"
#include "SimpleGraphics.h"
#include "Mesh.h"
#include "SimpleMaterial.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Reader.h"
#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


namespace Simple3DGraphics {


    SimpleGraphics::SimpleGraphics() {

    }


    SimpleGraphics::~SimpleGraphics() {
        delete[] this->camera;
    }


    bool SimpleGraphics::Open() {
        App::Open();
        this->window = new Display::Window;
        window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                this->window->Close();
            }
            if (key == GLFW_KEY_UP && mods != GLFW_MOD_SHIFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
                this->zPos -= 0.02f;
            }
            if (key == GLFW_KEY_DOWN && mods != GLFW_MOD_SHIFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
                this->zPos += 0.02f;
            }
            if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
                this->xPos -= 0.02f;
            }
            if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
                this->xPos += 0.02f;
            }
            if (key == GLFW_KEY_UP && mods == GLFW_MOD_SHIFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
                this->yPos += 0.02f;
            }
            if (key == GLFW_KEY_DOWN && mods == GLFW_MOD_SHIFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
                this->yPos -= 0.02f;
            }
        });

        if (this->window->Open()) {

            
            this->projection = glm::perspective(45.0f, (GLfloat) 200 / (GLfloat) 200, 0.1f, 10000.0f);

            this->camera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));


            Reader reader;
            GLchar* vst = (GLchar*) reader.readFile("/home/niklas/Desktop/D7045E/assigments/opengl-lab-env/projects/assigment3/code/SimpleShader.vert");
            GLchar* fst = (GLchar*) reader.readFile("/home/niklas/Desktop/D7045E/assigments/opengl-lab-env/projects/assigment3/code/SimpleShader.frag");

            std::shared_ptr<Shader> vShader(new Shader(vst, vertexShader));
            std::shared_ptr<Shader> fShader(new Shader(fst, fragmentShader));

            std::vector<std::shared_ptr<Shader>> shaders;
            shaders.push_back(vShader);
            shaders.push_back(fShader);

            std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram(shaders));
            this->shaderProgram = shaderProgram;


            // set clear color to gray
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            GLfloat red[3];
            red[0] = 1;
            red[1] = 0;
            red[2] = 0;

            GLfloat green[3];
            green[0] = 0;
            green[1] = 1;
            green[2] = 0;

            GLfloat blue[3];
            blue[0] = 0;
            blue[1] = 0;
            blue[2] = 1;

            GLfloat white[3];
            white[0] = 1;
            white[1] = 1;
            white[2] = 1;
        
            createBox(glm::vec3(0.3, 0, this->zPos), red);
            createBox(glm::vec3(-0.3, 0, this->zPos), green);
            createBox(glm::vec3(0, 0.3, this->zPos), blue);
            createBox(glm::vec3(0, -0.3, this->zPos), white);

            return true;
        }

        return false;
    }


    void SimpleGraphics::Run() {
        while (this->window->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT);
		    this->window->Update();

            this->shaderProgram->use();

            GLint projectionID = this->shaderProgram->getUniformId("projection");
            glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(this->projection));

            GLint cameraID = this->shaderProgram->getUniformId("camera");
            glUniformMatrix4fv(cameraID, 1, GL_FALSE, glm::value_ptr(this->camera->getCameraMatrix()));

            GLint lightID = this->shaderProgram->getUniformId("light");
            glm::vec3 cameraPos = this->camera->getCameraPos();
            glUniform3f(lightID, cameraPos[0], cameraPos[1], cameraPos[2]);


            glEnable(GL_CULL_FACE);  

            for(int i = 0; i < this->gNodes.size(); i++) {
                this->gNodes[i].setPitch((float)glfwGetTime());
                this->gNodes[i].setYawn((float)glfwGetTime());
                this->gNodes[i].setRoll((float)glfwGetTime());

                glm::vec3 pos = this->gNodes[i].getPosition();
                this->gNodes[i].setPosition(glm::vec3(pos[0] + this->xPos, pos[1] + this->yPos, this->zPos));

                this->gNodes[i].draw();
            }
            this->xPos = 0;
            this->yPos = 0;


            this->window->SwapBuffers();
        }
    }


    void SimpleGraphics::createGNode(std::vector<Vertex> vertices,  std::vector<unsigned int> indices, glm::vec3 pos, GLfloat color[3]) {
        std::shared_ptr<Mesh> mesh(new Mesh(vertices, indices));

        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(this->shaderProgram, color));      
    
        this->gNodes.push_back(GraphicsNode(mesh, simpleMaterial, pos, glm::vec3(1,1,1), 0, 0, 0));
    }


    void SimpleGraphics::createBox(glm::vec3 pos, GLfloat color[3]) {
        Vertex vertex0;
        vertex0.pos = {-0.1, -0.1, -0.1};
        Vertex vertex1;
        vertex1.pos = {0.1, -0.1, -0.1};
        Vertex vertex2;
        vertex2.pos = {0.1, 0.1, -0.1};
        Vertex vertex3;
        vertex3.pos = {-0.1, 0.1, -0.1};

        Vertex vertex4;
        vertex4.pos = {-0.1, -0.1, 0.1};
        Vertex vertex5;
        vertex5.pos = {0.1, -0.1, 0.1};
        Vertex vertex6;
        vertex6.pos = {0.1, 0.1, 0.1};
        Vertex vertex7;
        vertex7.pos = {-0.1, 0.1, 0.1};


        std::vector<Vertex> vertices;
        vertices.push_back(vertex0);
        vertices.push_back(vertex1);
        vertices.push_back(vertex2);
        vertices.push_back(vertex3);
        vertices.push_back(vertex4);
        vertices.push_back(vertex5);
        vertices.push_back(vertex6);
        vertices.push_back(vertex7);
        
        std::vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(2);

        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(4);
        indices.push_back(6);
        indices.push_back(7);

        indices.push_back(3);
        indices.push_back(6);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(7);
        indices.push_back(6);

        indices.push_back(2);
        indices.push_back(5);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(6);
        indices.push_back(5);

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(5);
        indices.push_back(0);
        indices.push_back(5);
        indices.push_back(4);

        indices.push_back(0);
        indices.push_back(7);
        indices.push_back(3);
        indices.push_back(0);
        indices.push_back(4);
        indices.push_back(7);


        createGNode(vertices, indices, pos, color);
    }

}
