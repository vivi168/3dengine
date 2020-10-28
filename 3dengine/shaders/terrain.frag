#version 330 core

in VertexData {
    vec2 texture_uv;
} i;

uniform sampler2D texture_sampler;

out vec3 out_color;

void main()
{
    out_color = texture(texture_sampler, i.texture_uv).rgb;
}
