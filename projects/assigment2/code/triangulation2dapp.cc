#include "config.h"
#include "triangulation2dapp.h"
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
"#define M_PI 3.1415926535897932384626433832795\n"
"uniform float dist;\n"
"uniform float angle;\n"
"void main()\n"
"{\n"
"   float newAngle = angle + M_PI * ((1.0/2.0) * (pos.x + 1.0 + pos.y + 1.0) * (pos.x + 1.0 + pos.y + 1.0 + 1.0) + (pos.y + 1.0)); \n" // Cantor pairing function https://en.wikipedia.org/wiki/Pairing_function
"   vec4 vDist = vec4((cos(newAngle) -sin(newAngle)) * dist, (sin(newAngle) + cos(newAngle)) * dist, 0, 0);\n"
"	gl_Position = vec4(pos, 1) + vDist;\n"
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

namespace Triangulation2d {


    Triangulation2dApp::Triangulation2dApp() {
        this->bufLength = 0;
        this->bufVBO = std::unique_ptr<GLfloat[]>(new GLfloat[this->bufLength]);
        this->showPoints = true;
        this->showConvexHull = false;
        this->showTriangulation = true;
        this->showTriangulationOutline = true;
        this->showC = false;
        this->angle = 0;
        this->dist = 0.0f;
    }
    

    Triangulation2dApp::~Triangulation2dApp() {
        if (this->vsBuffer) {
            delete[] this->vsBuffer;
        }
        if (this->fsBuffer) {
            delete[] this->fsBuffer;
        }
    }


    /**
     *   Initaial setup of the app.
     */
    bool Triangulation2dApp::Open() {
        App::Open();
        this->window = new Display::Window;

        srand (static_cast <unsigned> (time(0)));

        if (this->window->Open()) {
            // set clear color to gray
            glClearColor(0, 0, 0, 1.0f);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable( GL_BLEND );

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

            this->UpdateVBO();
            
            return true;
        }
        return false;
    }


    /**
     *  Main app loop. 
     */
    void Triangulation2dApp::Run() {
       while (this->window->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT);
            this->window->Update();

            this->angle += 0.05f;

            // do stuff
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
            glUseProgram(this->program);
            glLineWidth(3);
            glPointSize(10);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));


            GLuint angleID = glGetUniformLocation(this->program, "angle");
            GLuint distID = glGetUniformLocation(this->program, "dist");
 
            glUniform1f(angleID, this->angle);
            glUniform1f(distID, this->dist);


            int start = 0;
            if (this->showTriangulation) {
                glDrawArrays(GL_TRIANGLES, start, this->vertexcalc.getTriangulationLength() * 3);
            }
            
            start += this->vertexcalc.getTriangulationLength() * 3;

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            if (this->showTriangulationOutline) {
                glDrawArrays(GL_TRIANGLES, start, this->vertexcalc.getTriangulationLength() * 3);
            }
            
            start += this->vertexcalc.getTriangulationLength() * 3;

            if (this->showConvexHull) {
                glDrawArrays(GL_POLYGON, start, this->vertexcalc.getConvexHullLength());
            }
            start += this->vertexcalc.getConvexHullLength();
            
            if (this->showPoints) {
                glDrawArrays(GL_POINTS, start, this->vertexcalc.getPointsLength());
            }
            start += this->vertexcalc.getPointsLength();

            if (this->showC) {
                glDrawArrays(GL_POINTS, start, 1);
            }
            start += 1;
            
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            this->window->SwapBuffers();
        }
    }


    /**
     *  Updates the VBO with this->buf.
     */
    void Triangulation2dApp::UpdateVBO() {
        int lengthTriangulation = this->vertexcalc.getTriangulationLength() * 3;
        int lengthConvexHull = this->vertexcalc.getConvexHullLength();
        int lengthPoints = this->vertexcalc.getPointsLength();
        int lengthC = 1;
        int prev = 0;

        this->bufLength = (lengthTriangulation * 2 + lengthConvexHull + lengthPoints + lengthC) * 7;
        this->bufVBO = std::unique_ptr<GLfloat[]>(new GLfloat[this->bufLength]);


        // Adds triangulation.
        std::shared_ptr<VertexCalc::Triangle[]> triangulation = this->vertexcalc.getTriangulation();
        for (int i = 0; i < this->vertexcalc.getTriangulationLength(); i++) {
            VertexCalc::Point p1 = triangulation[i].p1;
            VertexCalc::Point p2 = triangulation[i].p2;
            VertexCalc::Point p3 = triangulation[i].p3;
            this->bufVBO[0 + i * 7 * 3 + prev] = p1.x;
            this->bufVBO[1 + i * 7 * 3 + prev] = p1.y;
            this->bufVBO[2 + i * 7 * 3 + prev] = p1.z;

            this->bufVBO[3 + i * 7 * 3 + prev] = p1.r;
            this->bufVBO[4 + i * 7 * 3 + prev] = p1.g;
            this->bufVBO[5 + i * 7 * 3 + prev] = p1.b;
            this->bufVBO[6 + i * 7 * 3 + prev] = 1;
            
            
            this->bufVBO[7 + i * 7 * 3 + prev] = p2.x;
            this->bufVBO[8 + i * 7 * 3 + prev] = p2.y;
            this->bufVBO[9 + i * 7 * 3 + prev] = p2.z;

            this->bufVBO[10 + i * 7 * 3 + prev] = p2.r;
            this->bufVBO[11 + i * 7 * 3 + prev] = p2.g;
            this->bufVBO[12 + i * 7 * 3 + prev] = p2.b;
            this->bufVBO[13 + i * 7 * 3 + prev] = 1;


            this->bufVBO[14 + i * 7 * 3 + prev] = p3.x;
            this->bufVBO[15 + i * 7 * 3 + prev] = p3.y;
            this->bufVBO[16 + i * 7 * 3 + prev] = p3.z;

            this->bufVBO[17 + i * 7 * 3 + prev] = p3.r;
            this->bufVBO[18 + i * 7 * 3 + prev] = p3.g;
            this->bufVBO[19 + i * 7 * 3 + prev] = p3.b;
            this->bufVBO[20 + i * 7 * 3 + prev] = 1;
        }
        prev += lengthTriangulation * 7;

        // Adds triangulation outline.
        for (int i = 0; i < this->vertexcalc.getTriangulationLength(); i++) {
            VertexCalc::Point p1 = triangulation[i].p1;
            VertexCalc::Point p2 = triangulation[i].p2;
            VertexCalc::Point p3 = triangulation[i].p3;
            this->bufVBO[0 + i * 7 * 3 + prev] = p1.x;
            this->bufVBO[1 + i * 7 * 3 + prev] = p1.y;
            this->bufVBO[2 + i * 7 * 3 + prev] = p1.z;

            this->bufVBO[3 + i * 7 * 3 + prev] = 1;
            this->bufVBO[4 + i * 7 * 3 + prev] = 1;
            this->bufVBO[5 + i * 7 * 3 + prev] = 1;
            this->bufVBO[6 + i * 7 * 3 + prev] = 1;
            
            
            this->bufVBO[7 + i * 7 * 3 + prev] = p2.x;
            this->bufVBO[8 + i * 7 * 3 + prev] = p2.y;
            this->bufVBO[9 + i * 7 * 3 + prev] = p2.z;

            this->bufVBO[10 + i * 7 * 3 + prev] = 1;
            this->bufVBO[11 + i * 7 * 3 + prev] = 1;
            this->bufVBO[12 + i * 7 * 3 + prev] = 1;
            this->bufVBO[13 + i * 7 * 3 + prev] = 1;


            this->bufVBO[14 + i * 7 * 3 + prev] = p3.x;
            this->bufVBO[15 + i * 7 * 3 + prev] = p3.y;
            this->bufVBO[16 + i * 7 * 3 + prev] = p3.z;

            this->bufVBO[17 + i * 7 * 3 + prev] = 1;
            this->bufVBO[18 + i * 7 * 3 + prev] = 1;
            this->bufVBO[19 + i * 7 * 3 + prev] = 1;
            this->bufVBO[20 + i * 7 * 3 + prev] = 1;
        }
        prev += lengthTriangulation * 7;

        // Adds convexHull.
        std::shared_ptr<VertexCalc::Point[]> convexHull = this->vertexcalc.getConvexHull();
        for (int i = 0; i < lengthConvexHull; i++) {
            this->bufVBO[0 + i * 7 + prev] = convexHull[i].x;
            this->bufVBO[1 + i * 7 + prev] = convexHull[i].y;
            this->bufVBO[2 + i * 7 + prev] = convexHull[i].z;

            this->bufVBO[3 + i * 7 + prev] = 1;
            this->bufVBO[4 + i * 7 + prev] = 0;
            this->bufVBO[5 + i * 7 + prev] = 0;
            this->bufVBO[6 + i * 7 + prev] = convexHull[i].a;
        }
        prev += lengthConvexHull * 7;

        // Adds all points
        std::shared_ptr<VertexCalc::Point[]> points = this->vertexcalc.getPoints();
        for (int i = 0; i < lengthPoints; i++) {
            this->bufVBO[0 + i * 7 + prev] = points[i].x;
            this->bufVBO[1 + i * 7 + prev] = points[i].y;
            this->bufVBO[2 + i * 7 + prev] = points[i].z;

            this->bufVBO[3 + i * 7 + prev] = points[i].r;
            this->bufVBO[4 + i * 7 + prev] = points[i].g;
            this->bufVBO[5 + i * 7 + prev] = points[i].b;
            this->bufVBO[6 + i * 7 + prev] = points[i].a;
        }
        prev += lengthPoints * 7;

        // Adds point pickedC
        VertexCalc::Point c = this->vertexcalc.getPickedC();
        this->bufVBO[0 + prev] = c.x;
        this->bufVBO[1 + prev] = c.y;
        this->bufVBO[2 + prev] = c.z;

        this->bufVBO[3 + prev] = c.r;
        this->bufVBO[4 + prev] = c.g;
        this->bufVBO[5 + prev] = c.b;
        this->bufVBO[6 + prev] = c.a;
        prev += lengthC * 7;

        // setup vbo
		glGenBuffers(1, &this->triangle);
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->bufLength * 7, static_cast<void *> (this->bufVBO.get()), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    
    /**
     *  Handels the GUI.
     */
    void Triangulation2dApp::RenderUI() {
        static bool showRead = false;
        static bool genPoints = false;
        static bool exit = false;
        static bool sameColor = false;
        static bool interpolationColor = false;
        static bool fourColor = false;
        static int pickCOption = 0;

        if (this->window->IsOpen()) {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    ImGui::MenuItem("read file", NULL, &showRead);
                    ImGui::MenuItem("Exit", NULL, &exit);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Gen Points")) {
                    ImGui::MenuItem("read file", NULL, &showRead);
                    ImGui::MenuItem("Random Points", NULL, &genPoints);
                    ImGui::RadioButton("Random", &pickCOption, 0); 
                    ImGui::RadioButton("Square Middle", &pickCOption, 1); 
                    ImGui::RadioButton("Rotating Calipers", &pickCOption, 2);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Show")) {
                    ImGui::MenuItem("Points", NULL, &this->showPoints);
                    ImGui::MenuItem("Convex Hull", NULL, &this->showConvexHull);
                    ImGui::MenuItem("C", NULL, &this->showC);
                    ImGui::MenuItem("Triangulation", NULL, &this->showTriangulation);
                    ImGui::MenuItem("Triangulation Outline", NULL, &this->showTriangulationOutline);
                    ImGui::SliderFloat("Distans", &this->dist, 0.0f, 0.5f);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Color")) {
                    ImGui::MenuItem("Same Color", NULL, &sameColor);
                    ImGui::MenuItem("Interpolation Color", NULL, &interpolationColor);
                    ImGui::MenuItem("Four Color", NULL, &fourColor);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
	    }

        if (showRead) this->ReaderUI(&showRead);
        if (genPoints) this->GenRandPointsUI(&genPoints);
        if (exit) this->window->Close();
        if (sameColor) {
            this->vertexcalc.colorSameColor(0, 0.5f, 1, 1);
            sameColor = false;
            this->UpdateVBO();
        }
        if (interpolationColor) {
            this->vertexcalc.colorInterpolationColor();
            interpolationColor = false;
            this->UpdateVBO();
        }
        if (fourColor) {
            this->vertexcalc.fourColor();
            fourColor = false;
            this->UpdateVBO();
        }
        this->vertexcalc.setPickCOption(pickCOption);
    }


    /**
     *  GUI for reading file. 
     */
    void Triangulation2dApp::ReaderUI(bool* open) {
        if (ImGui::Begin("File reader", open, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char buf[100] = "";
            ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
            if (ImGui::Button("Read file")) {
                std::string filePath(buf);
                this->vertexcalc.ReadPoints(filePath);
                this->UpdateVBO();
                *open = false;
            }
            ImGui::End();
        }
    }


    /**
     *  GUI for generating points. 
     */
    void Triangulation2dApp::GenRandPointsUI(bool* open) {
        if (ImGui::Begin("Rand point gen", open, ImGuiWindowFlags_AlwaysAutoResize)) {
            static int i0=3;
            ImGui::InputInt("input int", &i0, 1, 5);
            if (ImGui::Button("Gen Points")) {
                this->vertexcalc.GenRandomPoints(i0);
                this->UpdateVBO();
                *open = false;
            }
            ImGui::End();
        }
    }
}
