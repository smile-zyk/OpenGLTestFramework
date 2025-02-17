#version 460 core

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

const int RECTANGLE = 0;
const int CIRCLE = 1;

in vec4 frag_color;
in flat int frag_mode;

void main()
{
    if(frag_mode == RECTANGLE)
    {

    }
    else if(frag_mode == CIRCLE)
    {

    }
}