#version 430 core

layout(location = 0) in vec2 screen_uv;
layout(location = 1) in vec2 tex_coord;

out flat vec2 origin_tex_coord;
out vec2 frag_tex_coord;

void main()
{
    gl_Position = vec4(screen_uv * 2.0 - vec2(1.0), -1.0, 1.0);
    origin_tex_coord = tex_coord;
    frag_tex_coord = tex_coord;
}