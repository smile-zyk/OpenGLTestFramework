#version 460 core

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in int mode;
layout(location = 3) in vec3 parameter;
layout(location = 4) in int selected;

vec3 select_pos;

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
    float half_tri_edge = sqrt(3) * tri_radius;
    if(tri_idx == 1.0)
    {
        tri_offset = vec2(-half_tri_edge, -tri_radius);
    }
    else if(tri_idx == 2.0)
    {
        tri_offset = vec2(half_tri_edge, -tri_radius);
    }
    else if(tri_idx == 3.0)
    {
        tri_offset = vec2(0, 2 * tri_radius);
    }
    return position.xy + tri_offset;
}

float get_color_brightness(vec4 color)
{
    return color.r * 0.299 + color.g * 0.587 + color.b * 0.117;
}

vec4 brightened(vec4 color, float factor)
{
    return vec4(color.r * (1.0 - factor) + factor, color.g * (1.0 - factor) + factor, color.b * (1.0 - factor) + factor, color.a);
}

vec4 darkened(vec4 color, float factor)
{
    return vec4(color.r * (1 - factor), color.g * (1 - factor), color.b * (1 - factor), color.a);
}

vec4 get_select_color(vec4 origin_color)
{
    vec4 res;
    float factor = 0.5 + pow(get_color_brightness(origin_color), 3);
    factor = min(1.0, factor);
    res = brightened(origin_color, factor);

    if(abs(get_color_brightness(origin_color) - get_color_brightness(res)) < 0.05)
    {
        res = darkened(origin_color, 0.4);
        res.b = origin_color.b * (1.0 - factor) + factor;
    }

    return res;
}

void main()
{
    select_pos = position;
    if(mode == RECTANGLE)
    {
        gl_Position = projection_matrix * view_matrix * vec4(position, 1.0);
        frag_pos = (view_matrix * vec4(position, 1.0)).xy;
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
    if(selected == 1)
    {
        select_pos.z -= 2048;
        frag_color = get_select_color(color);
    }
}