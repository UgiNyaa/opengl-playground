#include <cmath>

#include "assimp_game.hpp"

glm::mat4 AssimpGame::model()
{
    return glm::rotate(t.full, glm::vec3(0, 1, 0));
}

glm::mat4 AssimpGame::view()
{
    glm::vec3 position(
        10,
        10,
        10
    );

    return glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 AssimpGame::projection()
{
    return glm::perspective
    (
        glm::radians(60.0f),
        1280.0f / 720.0f,
        0.1f, 100.0f
    );
}