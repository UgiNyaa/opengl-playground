#ifndef ASSIMP_GAME_HPP
#define ASSIMP_GAME_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../game.hpp"

class AssimpGame : public Game
{
private:
    GLuint pID;
    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint mvpID;
    GLuint tID;

    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;

    glm::mat4 model();
    glm::mat4 view();
    glm::mat4 projection();
public:
    virtual void initialize(int argc, char *argv[]) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void deinitialize() override;
};

#endif /* ASSIMP_GAME_HPP */