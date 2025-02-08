#version 430 core
in vec2 ftext_texture_coord;
in vec4 ftext_color;
out vec4 frag_color;
uniform sampler2D text_texture;

void main(){
    float a = texture(text_texture, ftext_texture_coord).r;
    frag_color = vec4(ftext_color.rgb, ftext_color.a * a);
}