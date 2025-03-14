#version 460 core

const int RECTANGLE = 0;
const int CIRCLE = 1;

in flat vec4 frag_color;
in flat int frag_mode;
in flat vec3 frag_parameter;
in flat vec2 circle_center;
in vec2 frag_pos;

uniform vec2 screen_size;

out vec4 res_color;

const int POINT_PATTERN = 0;
const int LINE_PATTERN = 1;

const float point_gap = 10.0;
const float point_size = 4.0;

bool pattern()
{
    vec2 uv = gl_FragCoord.xy / screen_size;
    vec2 derivative = fwidth(uv);
    vec2 pixel_dis = uv / derivative;
    vec2 uv_ratio = fract(pixel_dis / (point_size + point_gap));
    float point_ratio = point_size / (point_size + point_gap);
    if(uv_ratio.x > point_ratio || uv_ratio.y > point_ratio) return false;
    return true;
}

void main()
{
    if(pattern() == false) discard;
    if(frag_mode == RECTANGLE)
    {
        res_color = frag_color;
    }
    else if(frag_mode == CIRCLE)
    {
        float radius = frag_parameter.y;
        float dist = length(frag_pos - circle_center);
        if(dist < radius) res_color = frag_color;
        else discard;
    }
}