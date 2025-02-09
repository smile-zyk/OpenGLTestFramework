#version 430 core

out vec3 frag_pos;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

// opengl ndc space near plane is -1
vec2 grid_plane[6] = vec2[]
(
    vec2(1, 1), vec2(-1, -1), vec2(-1, 1),
    vec2(-1, -1), vec2(1, 1), vec2(1, -1)
);

vec3 ndc2world(vec3 p_ndc, mat4 view, mat4 proj)
{
    mat4 view_inv = inverse(view);
    mat4 proj_inv = inverse(proj);
    vec4 p_world = view_inv * proj_inv * vec4(p_ndc, 1.0);
    return p_world.xyz;
}

void main() 
{
    vec2 p = grid_plane[gl_VertexID];
    frag_pos = ndc2world(vec3(p.xy, 0.0), view_matrix, projection_matrix);
    gl_Position = vec4(p, 0.0, 1.0);
}
