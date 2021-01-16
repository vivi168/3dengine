#version 330 core

in VertexData {
    vec2 texture_uv;
} i;

uniform float delta;
uniform sampler2D texture_sampler;

out vec4 out_color;

void main()
{
    vec2 uv = i.texture_uv * 64.0f; // tile texture

    vec3 texcolor = texture(texture_sampler, uv).rgb;
    vec3 watercolor = vec3(0.0f, 0.0f, 1.0f);
    out_color = vec4(texcolor * watercolor, 0.6f);
    //out_color = vec4(0.0f, 0.0f, 1.0f, 0.3f);
}
