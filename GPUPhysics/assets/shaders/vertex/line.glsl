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
out vec3 lineColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float zoom;
uniform vec3 color;

vec3 get_position(int index) {
    return vec3(
        vertices[index].position[0],
        vertices[index].position[1],
        vertices[index].position[2]
    );
}

vec2 get_uv(int index){
    return vec2(
        vertices[index].uv[0],
        vertices[index].uv[1]
    );
}
void main()
{
    //gl_Position = projection * view * model * vec4(get_position(gl_VertexID), 1.0);
    gl_Position =  projection * view * vec4(get_position(gl_VertexID), 1.0);
    
    texCoord = get_uv(gl_VertexID);
    lineColor = color;
}

