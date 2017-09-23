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

Terrain::Terrain(uint32_t width, uint32_t height, float max_level, float stride)
    : width(width)
    , height(height)
    , max_level(max_level)
    , stride(stride)
{
    data.resize(width * height);
}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &GL.vertexbuffer);
    glDeleteBuffers(1, &GL.elementbuffer);
    glDeleteProgram(GL.pID);
}

uint32_t Terrain::get_height() const { return height * stride; }
uint32_t Terrain::get_width() const { return width * stride; }

float Terrain::get_level(float x, float z) const
{
	x /= stride;
	z /= stride;
	int x1 = std::floor(x);
	int x2 = std::floor(x) + 1;
	int z1 = std::floor(z);
	int z2 = std::floor(z) + 1;

	if (x2 == width)
	{
		x2--;
		x1--;
	}

	if (z2 == height)
	{
		z2--;
		z1--;
	}

	if (x2 == 0)
	{
		x2++;
		x1++;
	}

	if (z2 == 0)
	{
		z2++;
		z1++;
	}

	auto fq11 = data[z1 * width + x1];
	auto fq12 = data[z2 * width + x1];
	auto fq21 = data[z1 * width + x2];
	auto fq22 = data[z2 * width + x2];

	auto r1 = ((x2 - x) / (x2 - x1)) * fq11 + ((x - x1) / (x2 - x1)) * fq21;
	auto r2 = ((x2 - x) / (x2 - x1)) * fq12 + ((x - x1) / (x2 - x1)) * fq22;

	auto fxy = ((z2 - z) / (z2 - z1)) * r1 + ((z - z1) / (z2 - z1)) * r2;

	return fxy * max_level;
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
        GL.vertices.resize(width * height);
        GL.normals.resize(width * height);
        for (float z = 0; z < (height * stride); z += stride)
            for (float x = 0; x < (width * stride); x += stride)
            {
                int i = (x/stride) + (z/stride) * width;
                GL.vertices[i] = glm::vec3(x, get_level(x, z), z);

                auto hL = get_level(x - 1.0f, z - 0.0f);
                auto hR = get_level(x + 1.0f, z + 0.0f);
                auto hD = get_level(x - 0.0f, z - 1.0f);
                auto hU = get_level(x + 0.0f, z + 1.0f);
                GL.normals[i] = glm::vec3(hL - hR, 2.0, hD - hU);
                GL.normals[i] = glm::normalize(GL.normals[i]);
            }
        
        GL.indices.resize((width * 2) * (height - 1) + (height - 2));
        int index = 0;
        for (uint32_t z = 0; z < height - 1; z++)
        {
            if (z % 2 == 0)
            {
                uint32_t x;
                for (x = 0; x < width; x++)
                {
                    GL.indices[index++] = x + (z * width);
                    GL.indices[index++] = x + (z * width) + width;
                }
                if (z != height - 2)
                {
                    GL.indices[index++] = --x + (z * width);
                }
            }
            else
            {
                int x;
                for (x = width - 1; x >= 0; x--)
                {
                    GL.indices[index++] = x + (z * width);
                    GL.indices[index++] = x + (z * width) + width;
                }
                if (z != height - 2)
                {
                    GL.indices[index++] = ++x + (z * width);
                }
            }
        }
    }

    // buffer initialization
    {
        glGenVertexArrays(1, &GL.vao);
        glBindVertexArray(GL.vao);
        
        glGenBuffers(1, &GL.vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, GL.vertexbuffer);
        glBufferData
        (
            GL_ARRAY_BUFFER, 
            GL.vertices.size() * sizeof(glm::vec3), 
            GL.vertices.data(), 
            GL_STATIC_DRAW
        );
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &GL.normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, GL.normalbuffer);
        glBufferData
        (
            GL_ARRAY_BUFFER, 
            GL.normals.size() * sizeof(glm::vec3), 
            GL.normals.data(), 
            GL_STATIC_DRAW
        );
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

        glGenBuffers(1, &GL.elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL.elementbuffer);
        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,
            GL.indices.size() * sizeof(uint32_t),
            GL.indices.data(),
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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

    glBindVertexArray(GL.vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL.elementbuffer);
    glDrawElements
    (
        GL_TRIANGLE_STRIP,
        GL.indices.size(),
        GL_UNSIGNED_INT,
        (void*)0
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}