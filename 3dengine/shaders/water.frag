#version 330 core

//in VertexData {
//    vec2 texture_uv;
//} i;

// uniform sampler2D texture_sampler;

out vec4 out_color;

void main()
{
    // out_color = texture(texture_sampler, i.texture_uv).rgb;
    out_color = vec4(0.0f, 0.0f, 1.0f, 0.3f);
}
