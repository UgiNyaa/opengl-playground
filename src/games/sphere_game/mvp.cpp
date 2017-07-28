#include <cmath>

#include "sphere_game.hpp"

glm::mat4 SphereGame::model()
{
    return glm::rotate(t.full, glm::vec3(0, 1, 0));
}

glm::mat4 SphereGame::view()
{
    glm::vec3 position(
        camera.radius * std::sin(camera.theta) * std::cos(camera.phi),
        camera.radius * std::cos(camera.theta),
        camera.radius * std::sin(camera.theta) * std::sin(camera.phi)
    );

    return glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 SphereGame::projection()
{
    return glm::perspective
    (
        glm::radians(60.0f),
        1280.0f / 720.0f,
        0.1f, 100.0f
    );
}