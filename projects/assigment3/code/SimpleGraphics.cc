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
        window->SetKeyPressFunction([this](int32, int32, int32, int32) {
            this->window->Close();
        });

        if (this->window->Open()) {

            this->camera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

            // set clear color to gray
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            glm::vec3 pos = glm::vec3(0, 0, 1);

            GLfloat color[3];
            color[0] = 1;
            color[1] = 1;
            color[2] = 0;
            createSquare(pos, color);

            return true;
        }

        return false;
    }


    void SimpleGraphics::Run() {
        while (this->window->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT);
		    this->window->Update();


            this->gNodes[0].setRoll((float)glfwGetTime());


            this->shaderProgram->use();
            GLint cameraID = this->shaderProgram->getUniformId("camera");
            glUniformMatrix4fv(cameraID, 1, GL_FALSE, glm::value_ptr(this->camera->getCameraMatrix()));

            GLint lightID = this->shaderProgram->getUniformId("light");
            glm::vec3 cameraPos = this->camera->getCameraPos();
            glUniform3f(lightID, cameraPos[0], cameraPos[1], cameraPos[2]);

            this->gNodes[0].draw();


            this->window->SwapBuffers();
        }
    }


    void SimpleGraphics::createGNode(std::vector<Vertex> vertices,  std::vector<unsigned int> indices, glm::vec3 pos, GLfloat color[3]) {
        std::shared_ptr<Mesh> mesh(new Mesh(vertices, indices));

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

        std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, color));
        
    
        this->gNodes.push_back(GraphicsNode(mesh, simpleMaterial, pos, 0, 0, 0));
    }


    void SimpleGraphics::createSquare(glm::vec3 pos, GLfloat color[3]) {
        Vertex vertex0;
        vertex0.pos = {-0.1, -0.1, -0.05};
        Vertex vertex1;
        vertex1.pos = {0.1, -0.1, -0.05};
        Vertex vertex2;
        vertex2.pos = {0.1, 0.1, -0.05};
        Vertex vertex3;
        vertex3.pos = {-0.1, 0.1, -0.05};

        Vertex vertex4;
        vertex4.pos = {-0.1, -0.1, 0.05};
        Vertex vertex5;
        vertex5.pos = {0.1, -0.1, 0.05};
        Vertex vertex6;
        vertex6.pos = {0.1, 0.1, 0.05};
        Vertex vertex7;
        vertex7.pos = {-0.1, 0.1, 0.05};


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


        createGNode(vertices, indices, pos, color);
    }

}
