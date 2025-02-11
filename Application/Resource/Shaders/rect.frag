#version 430 core

in vec2 scene_pos;

out vec4 frag_color;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec2 rect_min;
uniform vec2 rect_max;

const int NO_OUTLINE = 0;
const int SOLID_OUTLINE = 1;
const int DASH_OUTLINE = 2;

int mode = SOLID_OUTLINE;
float dash_size = 8.f;
float gap_size = 4.f;
float outline_width = 2.f;
vec3 outline_color = vec3(1.f, 0.f, 0.f);
vec4 filled_color = vec4(0.8f, 0.8f, 0.8f, 0.5);

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

bool outline(vec2 scene_rect_min, vec2 scene_rect_max, vec2 scene_pos)
{
    vec2 derivative = fwidth(scene_pos);
    float min_x = scene_rect_min.x;
    float max_x = scene_rect_max.x;
    float min_y = scene_rect_min.y;
    float max_y = scene_rect_max.y;
    float to_v_pixel = min((scene_pos.x - min_x) / derivative.x, (max_x - scene_pos.x) /derivative.x);
    float to_h_pixel = min((scene_pos.y - min_y) / derivative.y, (max_y - scene_pos.y) /derivative.y);
    return to_h_pixel < outline_width || to_v_pixel < outline_width;
}

bool gap(vec2 origin_scene_pos, vec2 scene_pos)
{

}

void main()
{
    vec2 scene_rect_min = ndc2world(rect_min * 2.0 - 1.0, view_matrix, projection_matrix);
    vec2 scene_rect_max = ndc2world(rect_max * 2.0 - 1.0, view_matrix, projection_matrix);
    bool is_outline = outline(scene_rect_min, scene_rect_max, scene_pos);
    if(is_outline)
    {
        
    }
    else
    {
        frag_color = filled_color;
    }
}