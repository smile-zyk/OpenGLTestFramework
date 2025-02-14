#version 430 core
in vec3 frag_pos;

out vec4 frag_color;

uniform bool draw_x_axis;
uniform bool draw_y_axis;
uniform float axes_width;

float min_cell_size = 0.1f;
float min_cell_pixel = 5.f;

vec3 thick_color = vec3(0.4, 0.4, 0.4);
vec3 thin_color = vec3(0.3, 0.3, 0.3);
vec3 axis_x_color = vec3(0.6, 0.4, 0.4);
vec3 axis_y_color = vec3(0.4, 0.6, 0.4);

float log10(float x)
{
    return log(x) / log(10.0);
}

float max_vec2(vec2 v)
{
    return max(v.x, v.y);
}

float min_vec2(vec2 v)
{
    return min(v.x, v.y);
}

float remain(float x, float y)
{
    float r = mod(x, y);
    return min(r, y - r);
}

vec2 remain_vec2(vec2 pos, float y)
{
    return vec2(remain(pos.x, y), remain(pos.y, y));
}

vec4 grid(vec2 frag_pos)
{
    vec2 derivative = fwidth(frag_pos);

    float lod = max(0.0, log10((max_vec2(derivative) * min_cell_pixel) / min_cell_size) + 1.0);
    float fade = fract(lod);
    
    float lod0 = min_cell_size * pow(10, floor(lod));
    float lod1 = lod0 * 10;
    float lod2 = lod1 * 10;
    
    float half_line_width = axes_width / 2.f;

    float alpha0 = 1.0 - min_vec2(clamp(remain_vec2(frag_pos, lod0) / derivative / half_line_width, 0.0, 1.0));
    float alpha1 = 1.0 - min_vec2(clamp(remain_vec2(frag_pos, lod1) / derivative / half_line_width, 0.0, 1.0));
    float alpha2 = 1.0 - min_vec2(clamp(remain_vec2(frag_pos, lod2) / derivative / half_line_width, 0.0, 1.0));

    bool is_in_axis_x = draw_x_axis ? (abs(frag_pos.y) / derivative.y / half_line_width) < 1.0 : false;
    bool is_in_axis_y = draw_y_axis ? (abs(frag_pos.x) / derivative.x / half_line_width) < 1.0 : false;  
    
    return vec4(
        is_in_axis_x ? axis_x_color : is_in_axis_y ? axis_y_color : alpha2 > 0.0 ? thick_color : alpha1 > 0.0 ? mix(thick_color, thin_color, fade) : thin_color,
        alpha2 > 0.0 ? alpha2 : alpha1 > 0.0 ? alpha1 : alpha0 * (1.0 - fade)
    );
}

void main()
{
    frag_color = grid(frag_pos.xy);
}
