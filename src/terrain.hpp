#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <vector>
#include <cmath>
#include <cstdint>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/norm.hpp>

class Terrain
{
private:
    static const char* vertex_shader_code;
    static const char* fragment_shader_code;

    uint32_t width, height;
    float max_level;
    float stride;
    std::vector<float> data;

    struct {

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<uint32_t> indices;

        GLuint vao;
        GLuint vertexbuffer;
        GLuint normalbuffer;
        GLuint elementbuffer;

        GLuint pID;
        GLuint mvpID;
        GLuint colourmultiplierID;
    } GL;

public:
    Terrain(uint32_t width, uint32_t height, float max_level = 1, float stride = 1);
    ~Terrain();

    uint32_t get_width() const;
    uint32_t get_height() const;
    float get_level(float x, float z) const;

    void load(std::string imagepath);
    void initialize();
    void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    ) const;
};

#endif /* TERRAIN_HPP */