#version 450 core
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

layout(binding = 0, std430) buffer ssbo {
    VertexData vertices[];
};

layout(binding = 1, std430) buffer ssbo1 {
    ObjectData objects[];
};

out vec2 texCoord;
out vec4 vertex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float zoom;

vec3 get_position(int index) {
    return vec3(
        objects[index].center[0],
        objects[index].center[1],
        0.0
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

    gl_PointSize = 4.0 / zoom; 
    texCoord = get_uv(gl_VertexID);

    vertex_color = vec4(1.0,1.0,0.0,1.0);
}

