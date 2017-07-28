#ifndef COLLISION_GAME_HPP
#define COLLISION_GAME_HPP

#include "../../game.hpp"
#include "../../shape.hpp"

#include "../../colliding/colliding.hpp"

class CollisionGame : public Game
{
private:
    struct {
        std::unique_ptr<Shape> shape;

        glm::vec3 position = glm::vec3(0, -5, 0);
        glm::vec3 scale = glm::vec3(5, 1, 5);
    } ground;

    struct {
        std::unique_ptr<Shape> shape;

        float head_theta = 2;
        float head_phi = 0;
        float head_radius = 7;

        bool on_ground = false;

        float body_phi = 1;

        float acceleration = 40.0f;
        float max_speed = 10.0f;

        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 velocity = glm::vec3(0, 0, 0);
    } player;

    void player_update();

    glm::mat4 ground_model();
    glm::mat4 player_model();
    glm::mat4 view();
    glm::mat4 projection();

public:
    virtual void initialize(int argc, char *argv[]) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void deinitialize() override;

};

#endif /* COLLISION_GAME_HPP */