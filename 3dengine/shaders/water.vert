#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_uv;

//out VertexData {
//    vec2 texture_uv;
//} o;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(in_position, 1.0);
    //o.texture_uv = in_texture_uv;
}
