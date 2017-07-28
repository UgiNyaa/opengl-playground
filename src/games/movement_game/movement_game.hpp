#ifndef MOVEMENT_GAME_HPP
#define MOVEMENT_GAME_HPP

#include "../../game.hpp"
#include "../../shape.hpp"

class MovementGame : public Game
{
private:
    std::unique_ptr<Shape> block;

    std::unique_ptr<Shape> umwelt[8];

    struct {
        float head_theta = 2;
        float head_phi = 0;
        float head_radius = 7;

        bool on_ground = false;

        float body_phi = 1;

        float acceleration = 40.0f;
        float max_speed = 10.0f;

        glm::vec3 position = glm::vec3(0, 10, 0);
        glm::vec3 velocity = glm::vec3(0, 0, 0);
    } player;

    void player_update();

    glm::mat4 player_model();
    glm::mat4 view();
    glm::mat4 projection();

public:
    virtual void initialize(int argc, char *argv[]) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void deinitialize() override;
};

#endif /* MOVEMENT_GAME_HPP */