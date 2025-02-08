#version 430 core
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 text_texture_coord;
layout(location = 2) in vec4 text_color;
out vec2 ftext_texture_coord;
out vec4 ftext_color;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex_pos.xyz, 1.f);
    ftext_texture_coord = text_texture_coord;
    ftext_color = text_color;
}