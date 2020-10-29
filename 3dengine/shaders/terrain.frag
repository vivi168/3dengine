#version 330 core

in VertexData {
    vec2 texture_uv;
} i;

uniform sampler2D base_texture;
uniform sampler2D r_texture;
uniform sampler2D g_texture;
uniform sampler2D b_texture;
uniform sampler2D blendmap;

out vec4 out_color;

void main()
{
    vec4 blendmap_color = texture(blendmap, i.texture_uv);

    float intensity = 1 - (blendmap_color.r + blendmap_color.g + blendmap_color.b);
    vec2 uv = i.texture_uv * 64.0f; // tile texture
    vec4 base_texture_color = texture(base_texture, uv) * intensity;
    vec4 r_texture_color = texture(r_texture, uv) * blendmap_color.r;
    vec4 g_texture_color = texture(g_texture, uv) * blendmap_color.g;
    vec4 b_texture_color = texture(b_texture, uv) * blendmap_color.b;

    vec4 sum = base_texture_color + r_texture_color + g_texture_color + b_texture_color;
    out_color = sum;
}
