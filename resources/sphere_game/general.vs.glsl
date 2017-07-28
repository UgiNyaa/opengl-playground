#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform float t;
uniform mat4 MVP;

void main()
{
    vec3 v = vertexPosition_modelspace;
    float len = length(v);

    float a = ((1-len) / 2);
    float should_len = -cos(t*3)*a + (1-a);
    v *= vec3(1 / should_len);

    gl_Position = MVP * vec4(v, 1);
}