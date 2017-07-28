#include "collision_game.hpp"

void CollisionGame::initialize(int argc, char *argv[])
{
    Game::initialize(argc, argv);

    GLuint textureID;
    glGenTextures(1, &textureID);
    auto data = new uint8_t[4];
    data[0] = 255; data[1] = 255; data[2] = 255; data[3] = 255;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    player.shape = std::make_unique<Shape>("resources/movement_game/block.obj");
    player.shape->initialize();

    ground.shape = std::make_unique<Shape>("resources/movement_game/block.obj");
    ground.shape->initialize();
}

void CollisionGame::deinitialize()
{
    Game::deinitialize();
}

void CollisionGame::update()
{
    Game::update();

    player_update();
    
    int32_t width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2, height/2);
}

void CollisionGame::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    player.shape->draw
    (
        player_model(), 
        view(), 
        projection(), 
        glm::vec3(1, 1, 1)
    );

    glm::vec3 colour(.5f, .5f, .5f);
    if 
    (
        intersects
        (
            Box
            (
                player.position - glm::vec3(1),
                player.position + glm::vec3(1)
            ), 
            Box
            (
                ground.position - (ground.scale), 
                ground.position + (ground.scale)
            )
        )
    )
        colour.x += 0.5f;

    ground.shape->draw
    (
        ground_model(), 
        view(), 
        projection(), 
        colour
    );

    glfwSwapBuffers(window);
    glfwPollEvents();
}

