#version 330 core
out vec4 FragColor;

in vec3 Norm;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture0;
uniform vec3 inColor;

void main()
{
	FragColor = mix(texture(texture0, TexCoord), vec4(inColor,1.f), 0.8);
}