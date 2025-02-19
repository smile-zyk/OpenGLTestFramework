#version 460 core

const int RECTANGLE = 0;
const int CIRCLE = 1;

in flat vec4 frag_color;
in flat int frag_mode;
in flat vec3 frag_parameter;
in flat vec2 circle_center;
in vec2 frag_pos;

out vec4 res_color;

float circle_edge_smooth_pixel = 2;

void main()
{
    if(frag_mode == RECTANGLE)
    {
        res_color = frag_color;
    }
    else if(frag_mode == CIRCLE)
    {
        float radius = frag_parameter.y;
        float dist = length(frag_pos - circle_center);
        vec2 derivative_vec = fwidth(frag_pos);
        float derivative = max(derivative_vec.x, derivative_vec.y);
        float radius_pixel = radius / derivative;
        float dist_pixel = dist / derivative;
        float diff_pixel = radius_pixel - dist_pixel;
        float alpha = smoothstep(0, circle_edge_smooth_pixel, diff_pixel);
        if(alpha == 0.0) discard;
        res_color = vec4(frag_color.xyz, alpha * frag_color.w);
    }
}