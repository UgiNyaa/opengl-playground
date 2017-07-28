#include "movement_game.hpp"

void MovementGame::initialize(int argc, char *argv[])
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

    block = std::make_unique<Shape>("resources/movement_game/block.obj");
    block->initialize();

    for (size_t i = 0; i < 8; i++)
    {
        umwelt[i] = std::make_unique<Shape>("resources/movement_game/block.obj");
        umwelt[i]->initialize();
    }
}

void MovementGame::deinitialize()
{
    Game::deinitialize();
}

void MovementGame::update()
{
    Game::update();

    player_update();
    
    int32_t width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2, height/2);
}

void MovementGame::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    block->draw(player_model(), view(), projection(), glm::vec3(1, 1, 1));

    for (int i = 0; i < 8; i++)
    {
        auto model = glm::translate(glm::vec3((i-4) * 3, 0, (i % 2) * 10 - 5));
        umwelt[i]->draw(model, view(), projection(), glm::vec3(0.5f, 0.5f, 0.5f));
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

