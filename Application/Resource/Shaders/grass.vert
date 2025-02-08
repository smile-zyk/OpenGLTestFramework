#version 430 core
layout(location = 0) in vec3 rootPos;

void main()
{
    gl_Position = vec4(rootPos, 1.f);
}