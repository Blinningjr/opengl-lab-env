#include "Cube.h"


namespace Graphics3D {

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position):
        GraphicsNode(this->genMesh(size), material, position, glm::vec3(1,1,1), 0, 0, 0) {
        
        this->pitchSpeed = 0;
        this->rollSpeed = 0;
        this->yawnSpeed = 0;
        this->moveSpeed = 0;
        this->moveDistance = 0;
        this->originalY = position.y;
        this->angle = 0;
    }

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position, GLfloat yawn):
        GraphicsNode(this->genMesh(size), material, position, glm::vec3(1,1,1), 0, 0, yawn) {
        
        this->pitchSpeed = 0;
        this->rollSpeed = 0;
        this->yawnSpeed = 0;
        this->moveSpeed = 0;
        this->moveDistance = 0;
        this->originalY = position.y;
        this->angle = 0;
    }

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position,
        float pitchSpeed, float rollSpeed, float yawnSpeed): GraphicsNode(this->genMesh(size), material, position,
            glm::vec3(1,1,1), 0, 0, 0) {
        this->pitchSpeed = pitchSpeed;
        this->rollSpeed = rollSpeed;
        this->yawnSpeed = yawnSpeed;
        this->moveSpeed = 0;
        this->moveDistance = 0;
        this->originalY = position.y;
        this->angle = 0;
    }

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material, glm::vec3 position, GLfloat pitch, GLfloat roll,
        GLfloat yawn, float pitchSpeed, float rollSpeed, float yawnSpeed): 
            GraphicsNode(this->genMesh(size), material, position, glm::vec3(1,1,1), pitch, roll, yawn) {
        this->pitchSpeed = pitchSpeed;
        this->rollSpeed = rollSpeed;
        this->yawnSpeed = yawnSpeed;
        this->moveSpeed = 0;
        this->moveDistance = 0;
        this->originalY = position.y;
        this->angle = 0;
    }

    Cube::Cube(glm::vec3 size, std::shared_ptr<Material> material,  glm::vec3 position, GLfloat pitch, GLfloat roll,
        GLfloat yawn, float moveSpeed, float moveDistance): GraphicsNode(this->genMesh(size), material, position,
            glm::vec3(1,1,1), pitch, roll, yawn) {
        this->pitchSpeed = 0;
        this->rollSpeed = 0;
        this->yawnSpeed = 0;
        this->moveSpeed = moveSpeed;
        this->moveDistance = moveDistance;
        this->originalY = position.y;
        this->angle = 0;
    }

    Cube::~Cube() {
        
    }


    void Cube::update(float deltaTime) {
        this->pitch += this->pitchSpeed * deltaTime;
        this->roll += this->rollSpeed * deltaTime;
        this->yawn += this->yawnSpeed * deltaTime;
        this->angle += this->moveSpeed * deltaTime * 3.14;
        this->position.y = this->originalY + this->moveDistance * abs(sin(this->angle));
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

        glm::vec3 normal0 = glm::normalize(GraphicsNode::crossProduct(vertex2, vertex5, vertex1));
        glm::vec3 normal1 = glm::normalize(GraphicsNode::crossProduct(vertex0, vertex7, vertex3));
        glm::vec3 normal2 = glm::normalize(GraphicsNode::crossProduct(vertex3, vertex6, vertex2));
        glm::vec3 normal3 = glm::normalize(GraphicsNode::crossProduct(vertex0, vertex1, vertex5));
        glm::vec3 normal4 = glm::normalize(GraphicsNode::crossProduct(vertex4, vertex5, vertex6));
        glm::vec3 normal5 = glm::normalize(GraphicsNode::crossProduct(vertex0, vertex2, vertex1));


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
