#include "config.h"
#include "SimpleGraphics.h"
#include "Mesh.h"
#include "SimpleMaterial.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Reader.h"

#include <iostream>
#include <vector>


namespace Simple3DGraphics {


    SimpleGraphics::SimpleGraphics() {
        
    }


    SimpleGraphics::~SimpleGraphics() {
        delete[] this->gNode;
    }


    bool SimpleGraphics::Open() {
        App::Open();
        this->window = new Display::Window;
        window->SetKeyPressFunction([this](int32, int32, int32, int32) {
            this->window->Close();
        });

        if (this->window->Open()) {
            // set clear color to gray
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            Vertex vertex0;
            vertex0.pos = {-0.5, -0.5, -1};
            Vertex vertex1;
            vertex1.pos = {0.5, -0.5, -1};
            Vertex vertex2;
            vertex2.pos = {0, 0.5, -1};
            std::vector<Vertex> vertices;
            vertices.push_back(vertex0);
            vertices.push_back(vertex1);
            vertices.push_back(vertex2);
            
            std::vector<unsigned int> indices;
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);

            GLfloat color[3];
            color[0] = 1;
            color[1] = 0;
            color[2] = 0;

            glm::mat4 transform;

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

            std::shared_ptr<SimpleMaterial> simpleMaterial(new SimpleMaterial(shaderProgram, color));
            
        
            this->gNode = new GraphicsNode(mesh, simpleMaterial, transform);

            return true;
        }

        return false;
    }


    void SimpleGraphics::Run() {
        while (this->window->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT);
		    this->window->Update();


            this->gNode->draw();


            this->window->SwapBuffers();
        }
    }

}
