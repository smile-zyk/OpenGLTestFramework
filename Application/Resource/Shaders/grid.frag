#version 430 core
in vec3 frag_pos;

out vec4 frag_color;

uniform bool draw_axis;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

vec4 grid(vec3 frag_pos, float scale, bool draw_axis) 
{
    vec2 coord = frag_pos.xy * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumy = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.3, 0.3, 0.3, 1.0 - min(line, 1.0)); 
    if(draw_axis)
    {
        if(frag_pos.x < 0.1 * minimumx && frag_pos.x > -0.1 * minimumx)
            color.y = 0.6;
        if(frag_pos.y < 0.1 * minimumy && frag_pos.y > -0.1 * minimumy)
            color.x = 0.6;
    }
    return color;
}

void main()
{
    frag_color = grid(frag_pos, 1, true);
    if(frag_color.a == 0) discard;
}
