#version 330 core
out vec4 output;

in float color_d;

void main()
{
    output = vec4(max(color_d/150.0,0.03), 1, 1, 1) * vec4(0.3, 0.2, 0.9, 1.0);//vec4(1.0f, 0.5f, 0.2f, 1.0f);
}