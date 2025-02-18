#version 460 core

const int RECTANGLE = 0;
const int CIRCLE = 1;

in flat vec4 frag_color;
in flat int frag_mode;
in flat vec3 frag_parameter;
in flat vec2 circle_center;
in vec2 frag_pos;

out vec4 res_color;

void main()
{
    if(frag_mode == RECTANGLE)
    {
        res_color = frag_color;
    }
    else if(frag_mode == CIRCLE)
    {
        float radius = frag_parameter.y;
        if(length(frag_pos - circle_center) > radius) discard;
        res_color = frag_color;
    }
}