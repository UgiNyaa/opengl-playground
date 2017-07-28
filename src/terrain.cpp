#include <png++/png.hpp>

#include "terrain.hpp"

const char *Terrain::vertex_shader_code = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;

out vec3 normal_colour;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

    normal_colour = normal;
})";

const char *Terrain::fragment_shader_code = R"(
#version 330 core

in vec3 normal_colour;

out vec3 color;

uniform vec3 colourmultiplier;

void main()
{
    color.rgb = colourmultiplier * normal_colour;
}
)";

Terrain::Terrain(uint32_t width, uint32_t height, float max_level)
    : width(width)
    , height(height)
    , max_level(max_level)
{
    data.resize(width * height);
}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &GL.vertexbuffer);
    glDeleteBuffers(1, &GL.elementbuffer);
    glDeleteProgram(GL.pID);
}

uint32_t Terrain::get_height() const { return height; }
uint32_t Terrain::get_width() const { return width; }

float Terrain::get_level(float x, float z) const
{
    // TODO: linear interpolation
    return data[std::floor(x) + height * std::floor(z)] * max_level;
}

void Terrain::load(std::string imagepath)
{
    png::image<png::rgba_pixel> image(imagepath);

    width = image.get_width();
    height = image.get_height();

    data.resize(width * height);

    for (size_t z = 0; z < height; z++) 
        for (size_t x = 0; x < width; x++) 
            data[z * width + x + 0] = ((float)image.get_pixel(x, (height-z-1)).red / 255) * max_level;

}

void Terrain::initialize()
{
    // shader initialization
    {
        GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vsID, 1, &vertex_shader_code, NULL);
        glCompileShader(vsID);

        glShaderSource(fsID, 1, &fragment_shader_code, NULL);
        glCompileShader(fsID);

        GL.pID = glCreateProgram();
        glAttachShader(GL.pID, vsID);
        glAttachShader(GL.pID, fsID);
        glLinkProgram(GL.pID);
    }

    // vertex, uv, index data initialization
    {
        // TODO: use triangle stip
        GL.vertices.resize(height * width);
        GL.normals.resize(height * width);

        for (size_t z = 0; z < height; z++)
            for (size_t x = 0; x < width; x++)
            {
                auto root = x + z * height;
                auto x_next = (x+1) + z * height;
                auto z_next = x + (z + 1) * height;
                auto xz_next = (x+1) + (z+1) * height;

                GL.vertices[root] = glm::vec3(x, get_level(x, z), z);

                if (z < height - 1)
                {
                    GL.indices.push_back(root);
                    GL.indices.push_back(z_next);
                    GL.indices.push_back(xz_next);

                    GL.indices.push_back(root);
                    GL.indices.push_back(xz_next);
                    GL.indices.push_back(x_next);
                }

                auto hL = get_level(x - 1.0f, z - 0.0f);
                auto hR = get_level(x + 1.0f, z + 0.0f);
                auto hD = get_level(x - 0.0f, z - 1.0f);
                auto hU = get_level(x + 0.0f, z + 1.0f);

                GL.normals[root] = glm::vec3(hL - hR, 2.0f, hD - hU);
                GL.normals[root] = glm::normalize(GL.normals[root]);
            }

    }

    // buffer initialization
    {
        glGenBuffers(1, &GL.vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, GL.vertexbuffer);
        glBufferData
        (
            GL_ARRAY_BUFFER, 
            GL.vertices.size() * sizeof(glm::vec3), 
            GL.vertices.data(), 
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &GL.normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, GL.normalbuffer);
        glBufferData
        (
            GL_ARRAY_BUFFER, 
            GL.normals.size() * sizeof(glm::vec3), 
            GL.normals.data(), 
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &GL.elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL.elementbuffer);
        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,
            GL.indices.size() * sizeof(uint32_t),
            GL.indices.data(),
            GL_STATIC_DRAW
        );
    }

    GL.mvpID = glGetUniformLocation(GL.pID, "MVP");
    GL.colourmultiplierID = glGetUniformLocation(GL.pID, "colourmultiplier");
}

void Terrain::draw
(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection,
    glm::vec3 colourmultiplier
) const
{
    auto mvp = projection * view * model;

    glUseProgram(GL.pID);

    glUniformMatrix4fv(GL.mvpID, 1, GL_FALSE, &mvp[0][0]);
    glUniform3fv(GL.colourmultiplierID, 1, &colourmultiplier[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, GL.vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, GL.normalbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL.elementbuffer);
    glDrawElements
    (
        GL_TRIANGLES,
        GL.indices.size(),
        GL_UNSIGNED_INT,
        (void*)0
    );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}