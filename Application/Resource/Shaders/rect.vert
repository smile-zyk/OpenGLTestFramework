#version 430 core

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec2 rect_min;
uniform vec2 rect_max;

out vec2 scene_pos;

vec2 get_ndc_vertex(int vertex_id)
{
    switch(vertex_id)
    {
        case 0:
            return rect_min * 2.0 - 1.0;
        case 1:
            return vec2(rect_max.x, rect_min.y) * 2.0 - 1.0;
        case 2:
            return rect_max * 2.0 - 1.0;
        case 3:
            return rect_min * 2.0 - 1.0;
        case 4:
            return rect_max * 2.0 - 1.0;
        case 5:
            return vec2(rect_min.x, rect_max.y) * 2.0 - 1.0;
    }
}

vec4 ndc2world(vec4 p_ndc, mat4 view, mat4 proj)
{
    mat4 view_inv = inverse(view);
    mat4 proj_inv = inverse(proj);
    return view_inv * proj_inv * p_ndc;
}

void main()
{
    vec4 p_ndc = vec4(get_ndc_vertex(gl_VertexID), 1.0, 1.0);
    vec4 p_scene = ndc2world(p_ndc, view_matrix, projection_matrix);
    gl_Position = p_ndc;
    scene_pos = p_scene.xy;
}