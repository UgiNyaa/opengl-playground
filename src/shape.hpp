#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/norm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Shape
{
private:
    std::string path;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<uint32_t> indices;

    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint elementbuffer;

    GLuint pID;
    GLuint mvpID;
    GLuint colourmultiplierID;

public:
    Shape(std::string path);
    ~Shape();

    void initialize();
    void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    ) const;
};

#endif /* SHAPE_HPP */
