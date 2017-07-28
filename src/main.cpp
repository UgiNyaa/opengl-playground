#include "games/sphere_game/sphere_game.hpp"
#include "games/assimp_game/assimp_game.hpp"
#include "games/movement_game/movement_game.hpp"
#include "games/collision_game/collision_game.hpp"
#include "games/terrain_game/terrain_game.hpp"

int main(int argc, char *argv[])
{
    TerrainGame game;
    game.initialize(argc, argv);
    game.loop();
    game.deinitialize();
    exit(1);
}
