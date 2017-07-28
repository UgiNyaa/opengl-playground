#include <algorithm>

#include "game.hpp"

static bool GAME_INITIALIZED = false;

Game::~Game()
{
    deinitialize();
}

void Game::initialize(int argc, char *argv[])
{
    if (GAME_INITIALIZED)
    {
        std::cerr << "another initialized game is running" << '\n';
        exit(1);
    }

    GAME_INITIALIZED = true;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';
        exit(1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Game", NULL, NULL);
    window->game = this;

    if (window == nullptr)
    {
        std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials" << '\n';
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << '\n';
        exit(1);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &vertexarrayID);
    glBindVertexArray(vertexarrayID);

    open = true;
}

void Game::deinitialize()
{
    window->game = NULL;
    glDeleteVertexArrays(1, &vertexarrayID);
    glfwTerminate();
    glfwDestroyWindow(window);

    GAME_INITIALIZED = false;
}

void Game::update()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
        open = false;
}

void Game::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::loop()
{
    auto lastTime = glfwGetTime();

    while (open)
    {
        lastTime = glfwGetTime();
        update();
        draw();
        t.add(std::min(float(glfwGetTime() - lastTime), 0.2f));
    }
}
