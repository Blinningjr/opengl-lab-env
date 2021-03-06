//------------------------------------------------------------------------------
// snowflakeapp.cc
// (C) 2015-2017 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"

#include "snowflakeapp.h"
#include "snowflakeobj.h"
#include "slider.h"

#include <cstring>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <math.h>
#include <iostream>
#include <math.h>

#include <sstream>

const GLchar* vs =
	"#version 310 es\n"
	"precision mediump float;\n"
	"layout(location=0) in vec3 pos;\n"
	"layout(location=1) in vec4 color;\n"
	"layout(location=0) out vec4 Color;\n"
	"uniform mat4 rotate;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(pos, 1) * rotate;\n"
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

using namespace Display;
namespace Snowflake {

	//------------------------------------------------------------------------------
	/**
	*/
	SnowflakeApp::SnowflakeApp() {
		this->rotateMatrix = new GLfloat[16];
		
		GLfloat* matrix = new GLfloat[16];
		matrix[0] = 1.0f;	matrix[1] = 0;		matrix[2] = 0;		matrix[3] = 0;
		matrix[4] = 0;		matrix[5] = 1.0f;	matrix[6] = 0;		matrix[7] = 0;
		matrix[8] = 0;		matrix[9] = 0;		matrix[10] = 1.0f;	matrix[11] = 0;
		matrix[12] = 0;		matrix[13] = 0;		matrix[14] = 0;		matrix[15] = 1.0f;
		this->normalMatrix = matrix;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	SnowflakeApp::~SnowflakeApp() {
		this->snowFlake.~SnowflakeObj();
		delete[] this->rotateMatrix;
		delete[] this->normalMatrix;
	}

	//------------------------------------------------------------------------------
	/**
	*/


	bool SnowflakeApp::Open(){
		App::Open();
		this->window = new Display::Window;
		window->SetKeyPressFunction([this](int32, int32, int32, int32) {
			this->window->Close();
		});

		this->slider = new Slider(this->window, 0, 6, 0, -0.94f, 0.1f);

		this->snowFlake.setDepth(this->slider->getValue());


		if (this->window->Open()) {
			// set clear color to gray
			glClearColor(1.0f, 1.0f, 0.3f, 1.0f);

			// setup vertex shader
			this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLint length = (GLint)std::strlen(vs);
			glShaderSource(this->vertexShader, 1, &vs, &length);
			glCompileShader(this->vertexShader);

			// get error log
			GLint shaderLogSize;
			glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}

			// setup pixel shader
			this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			length = (GLint)std::strlen(ps);
			glShaderSource(this->pixelShader, 1, &ps, &length);
			glCompileShader(this->pixelShader);

			// get error log
			shaderLogSize;
			glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
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
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
				printf("[PROGRAM LINK ERROR]: %s", buf);
				delete[] buf;
			}

			glGenBuffers(1, &this->vertex);
			updateBuffer();

			return true;
		}
		return false;
	}

	void SnowflakeApp::updateBuffer() {
		if (this->slider->getNewValue()) {
			this->snowFlake.setDepth(this->slider->getValue());
		}

		//For rotating the snowflake.
		this->snowFlake.setAngle(this->snowFlake.getAngle() + 0.001f);

		float angle = this->snowFlake.getAngle();
		GLfloat* rotate = new GLfloat[16];
		rotate[0] = (GLfloat)cosf(angle * M_PI);	rotate[1] = (GLfloat)-sin(angle * M_PI);	rotate[2] = 0;		rotate[3] = 0;
		rotate[4] = (GLfloat)sin(angle * M_PI);		rotate[5] = (GLfloat)cos(angle * M_PI);		rotate[6] = 0;		rotate[7] = 0;
		rotate[8] = 0;								rotate[9] = 0;								rotate[10] = 1.0f;	rotate[11] = 0;
		rotate[12] = 0;								rotate[13] = 0;								rotate[14] = 0;		rotate[15] = 1.0f;
		

		delete[] this->rotateMatrix;
		this->rotateMatrix = rotate;

		GLfloat* t_buf1 = this->snowFlake.getSnowFlake();
		GLfloat* t_buf2 = this->snowFlake.getTriangleSnowFlake();

		int numFloats = this->snowFlake.getNumPoints() * 7;
		GLfloat buf[numFloats * 2 + 56];

		// Add color to the snowflakes innside.
		for (int i = 0; i < this->snowFlake.getNumPoints(); i++) {
			buf[i * 7] = t_buf2[i * 3];
			buf[1 + i * 7] = t_buf2[1 + i * 3];
			buf[2 + i * 7] = t_buf2[2 + i * 3];

			buf[3 + i * 7] = 1;
			buf[4 + i * 7] = 0;
			buf[5 + i * 7] = 0;
			buf[6 + i * 7] = 1;
		}

		// Add color to the snowflakes outline.
		for (int i = 0; i < this->snowFlake.getNumPoints(); i++) {
			buf[i * 7 + numFloats] = t_buf1[i * 3];
			buf[1 + i * 7 + numFloats] = t_buf1[1 + i * 3];
			buf[2 + i * 7 + numFloats] = t_buf1[2 + i * 3];

			buf[3 + i * 7 + numFloats] = 0;
			buf[4 + i * 7 + numFloats] = 0;
			buf[5 + i * 7 + numFloats] = 0;
			buf[6 + i * 7 + numFloats] = 1;
		}

		// Add color to the slider points.
		GLfloat* t_buf3 = this->slider->getSliderVertex();
		int blue = 1;
		for (int i = 0; i < 8; i++) {
			buf[i * 7 + numFloats * 2] = t_buf3[i * 3];
			buf[1 + i * 7 + numFloats * 2] = t_buf3[1 + i * 3];
			buf[2 + i * 7 + numFloats * 2] = t_buf3[2 + i * 3];

			if (i > 3){
				blue = 0;
			}

			buf[3 + i * 7 + numFloats * 2] = 0;
			buf[4 + i * 7 + numFloats * 2] = 0;
			buf[5 + i * 7 + numFloats * 2] = blue;
			buf[6 + i * 7 + numFloats * 2] = 1;
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->vertex);
		glClearBufferData(GL_ARRAY_BUFFER, GL_R32F, GL_RED, GL_FLOAT, buf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void SnowflakeApp::Run() {
		while (this->window->IsOpen()) {

			glClear(GL_COLOR_BUFFER_BIT);
			this->window->Update();

			updateBuffer();

			// do stuff
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindBuffer(GL_ARRAY_BUFFER, this->vertex);
			glUseProgram(this->program);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			GLuint MatrixID = glGetUniformLocation(this->program, "rotate");
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, this->rotateMatrix);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));
			
			
			glDrawArrays(GL_TRIANGLES, 0, this->snowFlake.getNumPoints());


			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(4);
			glDrawArrays(GL_POLYGON, this->snowFlake.getNumPoints(), this->snowFlake.getNumPoints());


			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, this->normalMatrix);
			glDrawArrays(GL_QUADS, this->snowFlake.getNumPoints() * 2, 8);

			// Unbind buffer
			glBindBuffer(GL_ARRAY_BUFFER, 0);


			this->window->SwapBuffers();
		}
	}

} // namespace Snowflake
