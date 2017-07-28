#include "collision_game.hpp"

void CollisionGame::player_update()
{
    double xpos, ypos;
    int32_t width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);
        
    bool go_forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool go_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool go_backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool go_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool moving = go_forward || go_backward || go_left || go_right;

    auto xdelta = float(width/2 - xpos);
    auto ydelta = float(height/2 - ypos);
    player.head_phi += t.delta * xdelta * -0.05f;
    player.head_theta += t.delta * ydelta * -0.05f;

    glm::vec3 acceleration;

    if (moving)
    {
        player.body_phi = player.head_phi;
        glm::vec3 forward
        (
            std::cos(player.head_phi), 
            0, 
            std::sin(player.head_phi)
        );
        glm::vec3 right
        (
            std::cos(player.head_phi + (3.14f / 2)),
            0,
            std::sin(player.head_phi + (3.14f / 2))
        );

        glm::vec3 direction;
        if (go_forward)
            direction += forward;
        
        if (go_right)
            direction += right;
        
        if (go_backward)
            direction += -forward;
        
        if (go_left)
            direction += -right;
        
        if (direction != glm::vec3(0, 0, 0))
            direction = glm::normalize(direction);

        acceleration += direction * player.acceleration;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && player.on_ground)
        acceleration.y += 1000 * t.delta;

    acceleration += glm::vec3(0, -5, 0);
    acceleration -= player.velocity * glm::vec3
    (
        player.acceleration / player.max_speed, 
        0.0f, 
        player.acceleration / player.max_speed
    );

    auto move = t.delta * (player.velocity + t.delta * acceleration * .5f);

    // collision
    player.on_ground = true;
    auto changed = correct
    (
        Box
        (
            player.position - glm::vec3(1),
            player.position + glm::vec3(1)
        ),
        move,
        Box
        (
            ground.position - ground.scale,
            ground.position + ground.scale
        )
    );

    std::cout << changed.y << '\n';
    if (changed.x)
        player.velocity.x = 0;
    if (changed.y)
        player.velocity.y = 0;
    if (changed.z)
        player.velocity.z = 0;

    player.position += move;
    player.velocity += t.delta * acceleration;
    std::cout << player.velocity.y << '\n';
    std::cout << player.position.y << '\n';
}