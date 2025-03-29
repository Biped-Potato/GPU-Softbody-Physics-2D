#version 450 core

out vec4 FragColor;

in vec4 vertex_color;
in vec2 texCoord;

uniform sampler2D texture1;

void main()
{
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    
    float dist = dot(coord, coord);
    
    if (dist > 1.0)
        discard;
    FragColor = vertex_color;
}