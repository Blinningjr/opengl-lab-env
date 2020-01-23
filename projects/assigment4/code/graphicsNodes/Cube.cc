#include "Cube.h"


namespace Graphics3D {

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position):
        GraphicsNode(this->genMesh(size), material, position, glm::vec3(1,1,1), 0, 0, 0) {
        
        this->pitchSpeed = 0;
        this->rollSpeed = 0;
        this->yawnSpeed = 0;
    }

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position,
        float pitchSpeed, float rollSpeed, float yawnSpeed): GraphicsNode(this->genMesh(size), material, position,
            glm::vec3(1,1,1), 0, 0, 0) {
        this->pitchSpeed = pitchSpeed;
        this->rollSpeed = rollSpeed;
        this->yawnSpeed = yawnSpeed;
    }

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material, glm::vec3 position, GLfloat pitch, GLfloat roll,
        GLfloat yawn, float pitchSpeed, float rollSpeed, float yawnSpeed): 
            GraphicsNode(this->genMesh(size), material, position, glm::vec3(1,1,1), pitch, roll, yawn) {
        this->pitchSpeed = pitchSpeed;
        this->rollSpeed = rollSpeed;
        this->yawnSpeed = yawnSpeed;
    }

    Cube::~Cube() {
        
    }


    void Cube::update(float deltaTime) {
        this->pitch += this->pitchSpeed * deltaTime;
        this->roll += this->rollSpeed * deltaTime;
        this->yawn += this->yawnSpeed * deltaTime;
    }


    /**
     *  Generates a cube mesh. 
     **/
    std::shared_ptr<Mesh> Cube::genMesh(glm::vec3 size) {
        float halfWidth = size.x/2;
        float halfHight = size.y/2;
        float halfDepth = size.z/2;

        glm::vec3 vertex0(-halfWidth, -halfHight, -halfDepth);
        glm::vec3 vertex1(halfWidth, -halfHight, -halfDepth);
        glm::vec3 vertex2(halfWidth, halfHight, -halfDepth);
        glm::vec3 vertex3(-halfWidth, halfHight, -halfDepth);

        glm::vec3 vertex4(-halfWidth, -halfHight, halfDepth);
        glm::vec3 vertex5(halfWidth, -halfHight, halfDepth);
        glm::vec3 vertex6(halfWidth, halfHight, halfDepth);
        glm::vec3 vertex7(-halfWidth, halfHight, halfDepth);

        glm::vec3 normal0(1, 0, 0);
        glm::vec3 normal1(-1, 0, 0);
        glm::vec3 normal2(0, 1, 0);
        glm::vec3 normal3(0, -1, 0);
        glm::vec3 normal4(0, 0, 1);
        glm::vec3 normal5(0, 0, -1);


        std::vector<Vertex> vertices;

        vertices.push_back({
            vertex0,
            normal5,
        });
        vertices.push_back({
            vertex2,
            normal5,
        });
        vertices.push_back({
            vertex1,
            normal5,
        });
        vertices.push_back({
            vertex0,
            normal5,
        });
        vertices.push_back({
            vertex3,
            normal5,
        });
        vertices.push_back({
            vertex2,
            normal5,
        });


        vertices.push_back({
            vertex4,
            normal4,
        });
        vertices.push_back({
            vertex5,
            normal4,
        });
        vertices.push_back({
            vertex6,
            normal4,
        });
        vertices.push_back({
            vertex4,
            normal4,
        });
        vertices.push_back({
            vertex6,
            normal4,
        });
        vertices.push_back({
            vertex7,
            normal4,
        });
        

        vertices.push_back({
            vertex3,
            normal2,
        });
        vertices.push_back({
            vertex6,
            normal2,
        });
        vertices.push_back({
            vertex2,
            normal2,
        });
        vertices.push_back({
            vertex3,
            normal2,
        });
        vertices.push_back({
            vertex7,
            normal2,
        });
        vertices.push_back({
            vertex6,
            normal2,
        });


        vertices.push_back({
            vertex2,
            normal0,
        });
        vertices.push_back({
            vertex5,
            normal0,
        });
        vertices.push_back({
            vertex1,
            normal0,
        });
        vertices.push_back({
            vertex2,
            normal0,
        });
        vertices.push_back({
            vertex6,
            normal0,
        });
        vertices.push_back({
            vertex5,
            normal0,
        });


        vertices.push_back({
            vertex0,
            normal3,
        });
        vertices.push_back({
            vertex1,
            normal3,
        });
        vertices.push_back({
            vertex5,
            normal3,
        });
        vertices.push_back({
            vertex0,
            normal3,
        });
        vertices.push_back({
            vertex5,
            normal3,
        });
        vertices.push_back({
            vertex4,
            normal3,
        });

        
        vertices.push_back({
            vertex0,
            normal1,
        });
        vertices.push_back({
            vertex7,
            normal1,
        });
        vertices.push_back({
            vertex3,
            normal1,
        });
        vertices.push_back({
            vertex0,
            normal1,
        });
        vertices.push_back({
            vertex4,
            normal1,
        });
        vertices.push_back({
            vertex7,
            normal1,
        });

        std::shared_ptr<Mesh> mesh(new Mesh(vertices));
        return mesh;
    }
}
