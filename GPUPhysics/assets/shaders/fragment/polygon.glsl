#version 450 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 polyColor;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(polyColor,1.0);
}