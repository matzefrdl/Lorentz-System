#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in float color;

out float color_d;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    color_d = color;
    float scale = 1;
    gl_Position = proj * view * vec4(vertex.x * scale, vertex.y * scale, vertex.z * scale, 1.0);
}