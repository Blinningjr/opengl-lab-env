#include "Simple3DGraphics.h"
#include "Mesh.h"
#include "SimpleMaterial.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Reader.h"


#include <vector>


namespace Simple3DGraphics {


    Simple3DGraphics::Simple3DGraphics() {
        
    }


    Simple3DGraphics::~Simple3DGraphics() {

    }


    bool Simple3DGraphics::Open() {
        App::Open();

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

        GLfloat color[3];
        color[0] = 1;
        color[1] = 0;
        color[2] = 0;

        glm::mat4 transform = {1.0f};
        
        
        vertices.push_back( );
        std::vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        Mesh mesh(vertices, indices);
        std::shared_ptr<Mesh> meshPointer(&mesh);

        Reader reader();

        std::shared_ptr<Shader> vertexShader = new Shader(reader->readFile("assigment3/code/SimpleShader.vert"), vertexShader);
        std::shared_ptr<Shader> fragmentShader = new Shader(reader->readFile("assigment3/code/SimpleShader.frag"), fragmentShader);

        std::vector<std::shared_ptr<Shader>> shaders;
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        std::shared_ptr<ShaderProgram> shaderProgram = new ShaderProgram(shaders);

        std::shared_ptr<SimpleMaterial> simpleMaterial = new SimpleMaterial(shaderProgram, color);
        
    
        this->gNode(meshPointer, simpleMaterial, transform);
        

        return false;
    }


    void Simple3DGraphics::Run() {
        this->gNode->draw();
    }

}
