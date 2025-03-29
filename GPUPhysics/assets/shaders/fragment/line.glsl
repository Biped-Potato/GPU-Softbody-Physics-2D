#version 450 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 lineColor;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(lineColor,1.0);
}