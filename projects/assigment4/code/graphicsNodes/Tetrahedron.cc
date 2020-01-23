#include "Tetrahedron.h"


namespace Graphics3D {


    Tetrahedron::Tetrahedron(float size, std::shared_ptr<Material> material,  glm::vec3 position):
            GraphicsNode(this->genMesh(size), material, position, glm::vec3(1,1,1), 0, 0, 0) {
        this->pitchSpeed = 0;
        this->rollSpeed = 0;
        this->yawnSpeed = 0;
    }

    Tetrahedron::Tetrahedron(float size, std::shared_ptr<Material> material,  glm::vec3 position, float pitchSpeed,
                float rollSpeed, float yawnSpeed): GraphicsNode(this->genMesh(size), 
                    material, position, glm::vec3(1,1,1), 0, 0, 0) {
        this->pitchSpeed = pitchSpeed;
        this->rollSpeed = rollSpeed;
        this->yawnSpeed = yawnSpeed;
    }


    Tetrahedron::Tetrahedron(float size, std::shared_ptr<Material> material,  glm::vec3 position,
        GLfloat pitch, GLfloat roll, GLfloat yawn, float pitchSpeed, float rollSpeed, float yawnSpeed): 
            GraphicsNode(this->genMesh(size), material, position, glm::vec3(1,1,1), pitch, roll, yawn) {
        this->pitchSpeed = pitchSpeed;
        this->rollSpeed = rollSpeed;
        this->yawnSpeed = yawnSpeed;
    }


    Tetrahedron::~Tetrahedron() {
        
    }


    void Tetrahedron::update(float deltaTime) {
        this->pitch += this->pitchSpeed * deltaTime;
        this->roll += this->rollSpeed * deltaTime;
        this->yawn += this->yawnSpeed * deltaTime;
    }


    glm::vec3 Tetrahedron::crossProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        glm::vec3 v = b - a;
        glm::vec3 u = c - a;
        glm::vec3 normal = {v.y * u.z - v.z * u.y,
                            -(v.x * u.z - v.z * u.x),
                            v.x * u.y - v.y * u.x};
        return normal;
    }


    /**
     *  Generates a Tetrahedron mesh. 
     **/
    std::shared_ptr<Mesh> Tetrahedron::genMesh(float size) {
        float r = size/2;
        float deltaX = sqrt(pow(size, 2.0f) + pow(r, 2.0f));
        glm::vec3 vertexA(-deltaX, -r, -r);
        glm::vec3 vertexB(deltaX, -r, -r);
        glm::vec3 vertexC(0, -r, size);
        glm::vec3 vertexD(0, size, 0);

        glm::vec3 normal0 = this->crossProduct(vertexA, vertexB, vertexC);
        glm::vec3 normal1 = this->crossProduct(vertexA, vertexD, vertexB);
        glm::vec3 normal2 = this->crossProduct(vertexB, vertexD, vertexC);
        glm::vec3 normal3 = this->crossProduct(vertexC, vertexD, vertexA);


        std::vector<Vertex> vertices;

        vertices.push_back({
            vertexA,
            normal0,
        });
        vertices.push_back({
            vertexB,
            normal0,
        });
        vertices.push_back({
            vertexC,
            normal0,
        });

        vertices.push_back({
            vertexA,
            normal1,
        });
        vertices.push_back({
            vertexD,
            normal1,
        });
        vertices.push_back({
            vertexB,
            normal1,
        });

        vertices.push_back({
            vertexB,
            normal2,
        });
        vertices.push_back({
            vertexD,
            normal2,
        });
        vertices.push_back({
            vertexC,
            normal2,
        });

        vertices.push_back({
            vertexC,
            normal3,
        });
        vertices.push_back({
            vertexD,
            normal3,
        });
        vertices.push_back({
            vertexA,
            normal3,
        });

        std::shared_ptr<Mesh> mesh(new Mesh(vertices));
        return mesh;
    }
}
