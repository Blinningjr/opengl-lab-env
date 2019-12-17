#include "Simple3DGraphics.h"
#include "Mesh.h"
#include "SimpleMaterial.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"

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
        
        
        vertices.push_back( );
        std::vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        Mesh mesh(vertices, indices);
        std::shared_ptr<Mesh> meshPointer(&mesh);

        Shader vertexShader(, vertexShader);
        Shader fragmentShader(, fragmentShader);

        ShaderProgram shaderProgram();

        SimpleMaterial simpleMaterial();
        
    
        this->gNode(meshPointer, );
        

        return false;
    }


    void Simple3DGraphics::Run() {
        this->gNode->draw();
    }

}
