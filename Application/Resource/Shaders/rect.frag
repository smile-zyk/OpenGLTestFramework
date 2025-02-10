#version 430 core

in flat vec2 origin_tex_coord;
in vec2 frag_tex_coord;

out vec4 frag_color;

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

float distance2outline(float value)
{
    return min(value, 1.0 - value);
}

vec2 distance2outline_vec2(vec2 v)
{
    return vec2(distance2outline(v.x), distance2outline(v.y));
}

void main()
{
    vec2 derivative = vec2(max(fwidth(frag_tex_coord.x), fwidth(frag_tex_coord.y)));
    if(mode == NO_OUTLINE)
    {
        frag_color = filled_color;
    }
    else
    {
        float outline_alpha = 1.0 - min_vec2(clamp(distance2outline_vec2(frag_tex_coord) / derivative / outline_width, 0.0, 1.0));
        if(outline_alpha > 0.0)
        {
            frag_color = vec4(outline_color, 1.0);
        }
        else
        {
            frag_color = filled_color;
        }
    }
}