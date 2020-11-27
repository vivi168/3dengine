#version 330 core

in VertexData {
    vec2 texture_uv;
} i;

uniform sampler2D texture_sampler;

out vec4 out_color;

void main()
{
    vec4 color = texture(texture_sampler, i.texture_uv);

    if (color.a < 0.1)
        discard;

    out_color = color;
}
