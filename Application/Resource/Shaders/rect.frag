#version 430 core

in vec2 scene_pos;

out vec4 frag_color;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec2 rect_min;
uniform vec2 rect_max;
uniform int mode;
uniform float dash_size;
uniform float gap_size;
uniform float outline_width;
uniform vec4 outline_color;
uniform vec4 filled_color;

const int NO_OUTLINE = 0;
const int SOLID_OUTLINE = 1;
const int DASH_OUTLINE = 2;

float max_vec2(vec2 v)
{
    return max(v.x, v.y);
}

float min_vec2(vec2 v)
{
    return min(v.x, v.y);
}

vec2 ndc2world(vec2 p_ndc, mat4 view, mat4 proj)
{
    mat4 view_inv = inverse(view);
    mat4 proj_inv = inverse(proj);
    return (view_inv * proj_inv * vec4(p_ndc, 1.0, 1.0)).xy;
}

bool outline(vec2 scene_rect_min, vec2 scene_rect_max, vec2 origin_scene_pos, vec2 scene_pos)
{
    vec2 derivative = fwidth(scene_pos);
    float min_x = scene_rect_min.x;
    float max_x = scene_rect_max.x;
    float min_y = scene_rect_min.y;
    float max_y = scene_rect_max.y;
    float to_y_pixel = min((scene_pos.x - min_x) / derivative.x, (max_x - scene_pos.x) /derivative.x);
    float to_x_pixel = min((scene_pos.y - min_y) / derivative.y, (max_y - scene_pos.y) /derivative.y);
    bool is_in_y_outline = to_y_pixel < outline_width;
    bool is_in_x_outline = to_x_pixel < outline_width;
    if(is_in_y_outline || is_in_x_outline)
    {
        if(mode == DASH_OUTLINE)
        {
            if(is_in_y_outline)
            {
                float to_origin_x_pixel = (scene_pos.y - origin_scene_pos.y) / derivative.y;
                if(fract(to_origin_x_pixel / (dash_size + gap_size)) > dash_size / (dash_size + gap_size))
                    return false;
            }
            if(is_in_x_outline)
            {
                float to_origin_y_pixel = (scene_pos.x - origin_scene_pos.x) / derivative.x;
                if(fract(to_origin_y_pixel / (dash_size + gap_size)) > dash_size / (dash_size + gap_size))
                    return false;
            }
        }
        return true;
    }
}

void main()
{
    vec2 scene_rect_min = ndc2world(rect_min * 2.0 - 1.0, view_matrix, projection_matrix);
    vec2 scene_rect_max = ndc2world(rect_max * 2.0 - 1.0, view_matrix, projection_matrix);
    bool is_outline = mode == NO_OUTLINE ? false : outline(scene_rect_min, scene_rect_max, scene_rect_min, scene_pos);
    if(is_outline)
    {
        frag_color = outline_color;
    }
    else
    {
        frag_color = filled_color;
    }
}