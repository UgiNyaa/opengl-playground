#ifndef TERRAIN_GAME_HPP
#define TERRAIN_GAME_HPP

#include "../../game.hpp"
#include "../../shape.hpp"
#include "../../terrain.hpp"

#include "../../colliding/colliding.hpp"

class TerrainGame : public Game
{
private:
    std::unique_ptr<Terrain> terrain;

    struct {
        std::unique_ptr<Shape> shape;

        float head_theta = 2;
        float head_phi = 0;
        float head_radius = 7;

        bool on_ground = false;

        float body_phi = 1;

        float acceleration = 40.0f;
        float max_speed = 10.0f;

        glm::vec3 position = glm::vec3(0, 5, 0);
        glm::vec3 velocity = glm::vec3(0, 0, 0);
    } player;

    void update_player();

    glm::mat4 player_model();
    glm::mat4 view();
    glm::mat4 projection();
    
public:
    virtual void initialize(int argc, char *argv[]) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void deinitialize() override;
};

#endif /* TERRAIN_GAME_HPP */