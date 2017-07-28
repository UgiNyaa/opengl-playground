#include <cmath>

#include "collision_game.hpp"

glm::mat4 CollisionGame::ground_model()
{
    return glm::translate(glm::vec3(0.0f, -5.0f, 0.0f)) 
      * glm::scale(glm::vec3(5.0f, 1.0f, 5.0f));
}

glm::mat4 CollisionGame::player_model()
{
    return glm::translate(player.position);
}

glm::mat4 CollisionGame::view()
{
    glm::vec3 up(
        player.head_radius * 
        std::sin(player.head_theta - (3.14 / 2)) * 
        std::cos(player.head_phi),
        
        player.head_radius * 
        std::cos(player.head_theta - (3.14 / 2)),
        
        player.head_radius * 
        std::sin(player.head_theta - (3.14 / 2)) * 
        std::sin(player.head_phi)
    );

    glm::vec3 direction(
        player.head_radius * 
        std::sin(player.head_theta) * 
        std::cos(player.head_phi),
        
        player.head_radius * 
        std::cos(player.head_theta),
        
        player.head_radius * 
        std::sin(player.head_theta) * 
        std::sin(player.head_phi)
    );

    return glm::lookAt(
        player.position - direction, 
        player.position, 
        up
    );
}

glm::mat4 CollisionGame::projection()
{
    return glm::perspective
    (
        glm::radians(60.0f),
        1280.0f / 720.0f,
        0.1f, 100.0f
    );
}