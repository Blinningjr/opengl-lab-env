#include "config.h"
#include "triangulation3dapp.h"
#include <cstring>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <math.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "imgui.h"

const GLchar* vs =
"#version 310 es\n"
"precision mediump float;\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 310 es\n"
"precision mediump float;\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

namespace Triangulation3d {
    Triangulation3dApp::Triangulation3dApp() {
        this->bufLength = 0;
        this->buf = new GLfloat[this->bufLength];
    }
    
    Triangulation3dApp::~Triangulation3dApp() {
        delete[] this->buf;
        delete[] this->vsBuffer;
        delete[] this->fsBuffer;
    }

    bool Triangulation3dApp::Open() {
        App::Open();
        this->window = new Display::Window;
        // window->SetKeyPressFunction([this](int32 a, int32 b, int32 c, int32 d) {
        //     this->window->Close();--
        // });

        srand (static_cast <unsigned> (time(0)));

        if (this->window->Open()) {
            // set clear color to gray
            glClearColor(1.0f, 1.0f, 0.3f, 1.0f);

            // setup vertex shader
            this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
            this->vsBufferLength = (GLint)std::strlen(vs);
            this->vsBuffer = new GLchar[this->vsBufferLength];
            for (int i = 0; i < this->vsBufferLength; i++) {
                this->vsBuffer[i] = vs[i];
            }
            glShaderSource(this->vertexShader, 1, &this->vsBuffer, &this->vsBufferLength);
            glCompileShader(this->vertexShader);

            // get error log
            GLint shaderLogSize;
            glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
            if (shaderLogSize > 0)
            {
                GLchar* buf = new GLchar[shaderLogSize];
                glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
                printf("[SHADER COMPILE ERROR]: %s", buf);
                delete[] buf;
            }

            // setup pixel shader
            this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
            this->fsBufferLength = (GLint)std::strlen(ps);
            this->fsBuffer = new GLchar[this->fsBufferLength];
            for (int i = 0; i < this->fsBufferLength; i++) {
                this->fsBuffer[i] = ps[i];
            }
            glShaderSource(this->pixelShader, 1, &this->fsBuffer, &this->fsBufferLength);
            glCompileShader(this->pixelShader);

            // get error log
            shaderLogSize;
            glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
            if (shaderLogSize > 0)
            {
                GLchar* buf = new GLchar[shaderLogSize];
                glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
                printf("[SHADER COMPILE ERROR]: %s", buf);
                delete[] buf;
            }

            // create a program object
            this->program = glCreateProgram();
            glAttachShader(this->program, this->vertexShader);
            glAttachShader(this->program, this->pixelShader);
            glLinkProgram(this->program);
            glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
            if (shaderLogSize > 0)
            {
                GLchar* buf = new GLchar[shaderLogSize];
                glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
                printf("[PROGRAM LINK ERROR]: %s", buf);
                delete[] buf;
            }

            // set ui rendering function
            this->window->SetUiRender([this]()
            {
                this->RenderUI();
            });
            
            return true;
        }
        return false;
    }


    void Triangulation3dApp::Run() {
       while (this->window->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT);
            this->window->Update();

            // do stuff
            this->UpdateVBO();

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
            glUseProgram(this->program);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));
            glDrawArrays(GL_LINE_LOOP, 0, this->bufLength/7);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            this->window->SwapBuffers();
        }
    }


    void Triangulation3dApp::UpdateVBO() {
        // setup vbo
		glGenBuffers(1, &this->triangle);
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->bufLength, this->buf, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    void Triangulation3dApp::ReadPoints() {
        int numCords = this->reader.getPointsLength()/2;
        GLfloat* tBuf = this->reader.getPoints();

        delete[] this->buf;
        this->bufLength = numCords * 7;
        this->buf = new GLfloat[bufLength];

        for (int i = 0; i < numCords; i++) {
            this->buf[0 + i * 7] = tBuf[0 + i * 2];
            this->buf[1 + i * 7] = tBuf[1 + i * 2];
            this->buf[2 + i * 7] = -1;

            this->buf[3 + i * 7] = 0;
            this->buf[4 + i * 7] = 0;
            this->buf[5 + i * 7] = 0;
            this->buf[6 + i * 7] = 1;
        }
    }

    void Triangulation3dApp::GenRandomPoints(int numPoints) {
        if (numPoints < 3) {
            numPoints = 3;
        }

        delete[] this->buf;
        this->bufLength = numPoints * 7;
        this->buf = new GLfloat[bufLength];

        for (int i = 0; i < numPoints; i++) {
            this->buf[0 + i * 7] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)) - 1.0f;
            this->buf[1 + i * 7] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)) - 1.0f;
            this->buf[2 + i * 7] = -1;

            this->buf[3 + i * 7] = 0;
            this->buf[4 + i * 7] = 0;
            this->buf[5 + i * 7] = 0;
            this->buf[6 + i * 7] = 1;
        }
    }
    
    /**
     *  Handels the GUI.
     */
    void Triangulation3dApp::RenderUI() {
        static bool showRead = false;
        static bool genPoints = false;

        if (this->window->IsOpen()) {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    ImGui::MenuItem("read file", NULL, &showRead);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Gen Points")) {
                    ImGui::MenuItem("read file", NULL, &showRead);
                    ImGui::MenuItem("Random Points", NULL, &genPoints);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
	    }

        if (showRead) this->ReaderUI(&showRead);
        if (genPoints) this->GenRandPointsUI(&genPoints);
    }


    /**
     *  GUI for reading file. 
     */
    void Triangulation3dApp::ReaderUI(bool* open) {
        if (ImGui::Begin("File reader", open, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char buf[100] = "";
            ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
            if (ImGui::Button("Read file")) {
                std::string filePath(buf);
                this->reader.ReadPoints(filePath);
                this->ReadPoints();
                *open = false;
            }
            ImGui::End();
        }
    }


    /**
     *  GUI for generating points. 
     */
    void Triangulation3dApp::GenRandPointsUI(bool* open) {
        if (ImGui::Begin("Rand point gen", open, ImGuiWindowFlags_AlwaysAutoResize)) {
            static int i0=3;
            ImGui::InputInt("input int", &i0, 1, 5);
            if (ImGui::Button("Gen Points")) {
                this->GenRandomPoints(i0);
                *open = false;
            }
            ImGui::End();
        }
    }

}
