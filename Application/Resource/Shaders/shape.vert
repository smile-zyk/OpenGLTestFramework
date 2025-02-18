#version 460 core

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in int mode;
layout(location = 3) in vec3 parameter;

const int RECTANGLE = 0;
const int CIRCLE = 1;

// parameter 
// circle:
// parameter.x = triangle index
// parameter.y = radius

out flat vec4 frag_color;
out flat int frag_mode;
out flat vec3 frag_parameter;
out flat vec2 circle_center;
out vec2 frag_pos;

vec2 get_circle_triangle_position()
{
    vec2 tri_offset;
    float tri_idx = parameter.x;
    float tri_radius = parameter.y;
    float tri_edge_length = sqrt(3);
    if(tri_idx == 1.0)
    {
        tri_offset = vec2(-tri_edge_length / 2.f, -tri_radius);
    }
    else if(tri_idx == 2.0)
    {
        tri_offset = vec2(tri_edge_length / 2.f, -tri_radius);
    }
    else if(tri_idx == 3.0)
    {
        tri_offset = vec2(0, 2 * tri_radius);
    }
    return position.xy + tri_offset;
}

void main()
{
    if(mode == RECTANGLE)
    {
        gl_Position = projection_matrix * view_matrix * vec4(position, 1.0);
    }
    else if(mode == CIRCLE)
    {
        circle_center = position.xy;
        vec4 tri_pos = vec4(get_circle_triangle_position(), position.z, 1.0);
        gl_Position = projection_matrix * view_matrix * tri_pos, 1.0;
        frag_pos = tri_pos.xy;
    }
    frag_color = color;
    frag_mode = mode;
    frag_parameter = parameter;
}