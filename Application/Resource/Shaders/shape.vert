#version 460 core

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in int mode;

out vec4 frag_color;
out flat int frag_mode;

void main()
{
    gl_Position = vec4(position, 1.0);
    frag_color = frag_color;
    frag_mode = mode;
}