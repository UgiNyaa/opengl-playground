#ifndef SPHERE_GAME_HPP
#define SPHERE_GAME_HPP

#include "../../game.hpp"
#include "camera.hpp"

class SphereGame : public Game
{
private:
    static const GLfloat g_vertex_buffer_base[];
    static const unsigned int g_index_buffer_base[];

    std::vector<glm::vec3> vertices{
        glm::vec3(0.0f, 1.0f, 0.0f),

        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, -1.0f),
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(-1.0f, 0.0f, 1.0f),

        glm::vec3(0.0f, -1.0f, 0.0f),
    };
    std::vector<uint32_t> indices{
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        
        5, 2, 1,
        5, 3, 2,
        5, 4, 3,
        5, 1, 4,
    };

    Camera camera;

    GLuint pID;
    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint mvpID;
    GLuint tID;

    glm::mat4 model();
    glm::mat4 view();
    glm::mat4 projection();
    void dig(int depth);
public:
    virtual void initialize(int argc, char *argv[]) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void deinitialize() override;
};

#endif /* SPHERE_GAME_HPP */