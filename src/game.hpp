#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/norm.hpp>

#include "time.hpp"

struct GLFWwindow
{
    class Game* game;
};

class Game
{
protected:
    Time t;
    bool open = false;

    GLFWwindow* window;
    GLuint vertexarrayID;

public:
    Game() { };
    virtual ~Game();

    virtual void initialize(int argc, char *argv[]);
    virtual void update();
    virtual void draw();
    virtual void deinitialize();

    void loop();
};

#endif /* end of include guard: GAME_HPP */
