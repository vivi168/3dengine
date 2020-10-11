#version 330 core

in vec3 color;
in vec2 UV;

out vec3 FragColor;

uniform sampler2D texture_sampler;

void main()
{
    // FragColor = color;
    FragColor = texture(texture_sampler, UV).rgb;
}
