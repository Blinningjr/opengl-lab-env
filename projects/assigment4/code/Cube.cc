#include "Cube.h"


namespace Graphics3D {

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position):
        GraphicsNode(this->genMesh(size.x, size.y, size.z), material, position, glm::vec3(1,1,1), 0, 0, 0) {

    }

    Cube::Cube(float width, float hight, float depth, std::shared_ptr<Material> material,  glm::vec3 position):
        GraphicsNode(this->genMesh(width, hight, depth), material, position, glm::vec3(1,1,1), 0, 0, 0) {

    }

    Cube::Cube(float width, float hight, float depth, std::shared_ptr<Material> material,  glm::vec3 position,
        glm::vec3 scale, GLfloat pitch, GLfloat roll, GLfloat yawn): 
        GraphicsNode(this->genMesh(width, hight, depth), material, position, scale, pitch, roll, yawn) {
    }

    Cube::~Cube() {
        
    }


    /**
     *  Generates a cube mesh. 
     **/
    std::shared_ptr<Mesh> Cube::genMesh(float width, float hight, float depth) {
        float halfWidth = width/2;
        float halfHight = hight/2;
        float halfDepth = depth/2;

        Vertex vertex0;
        vertex0.pos = {-halfWidth, -halfHight, -halfDepth};
        Vertex vertex1;
        vertex1.pos = {halfWidth, -halfHight, -halfDepth};
        Vertex vertex2;
        vertex2.pos = {halfWidth, halfHight, -halfDepth};
        Vertex vertex3;
        vertex3.pos = {-halfWidth, halfHight, -halfDepth};

        Vertex vertex4;
        vertex4.pos = {-halfWidth, -halfHight, halfDepth};
        Vertex vertex5;
        vertex5.pos = {halfWidth, -halfHight, halfDepth};
        Vertex vertex6;
        vertex6.pos = {halfWidth, halfHight, halfDepth};
        Vertex vertex7;
        vertex7.pos = {-halfWidth, halfHight, halfDepth};


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

        std::shared_ptr<Mesh> mesh(new Mesh(vertices, indices));
        return mesh;
    }
}
