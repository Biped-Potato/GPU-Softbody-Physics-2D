#version 450 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

layout(binding = 0, std430) buffer ssbo {
    VertexData vertices[];
};

layout(binding = 1, std430) buffer ssbo1 {
    ObjectData objects[];
};

layout(binding = 2, std430) buffer ssbo2 {
    Spring springs[];
};

out vec2 texCoord;
out vec3 polyColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float zoom;
uniform vec3 color;
/*
0 top left
1 bottom left
2 bottom right
3 top right
4 bottom right
5 top left
*/
const uint indices[6] = uint[6](
    0,4,6,2,6,0
);
const float texCoords[8] = float[8] (
    0.0, 1.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 0.0
);

void main()
{
    uint index = gl_VertexID;
    ObjectData o = objects[uint(index / 6)];
    uint local = index % 6;
    uint start = indices[local];

    vec2 uv = vec2(texCoords[start],texCoords[start+1]);
    vec2 position = vec2(o.bounds[start],o.bounds[start+1]);

    gl_Position =  projection * view * vec4(position,0.0, 1.0);
    texCoord = position;

    polyColor = color;
}

