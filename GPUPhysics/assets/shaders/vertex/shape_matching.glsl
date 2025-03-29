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
    VertexData v = vertices[index];

    vec2 center = vec2(objects[v.o_id].center[0],objects[v.o_id].center[1]);
    vec2 global = vec2(v.position[0],v.position[1]) - center;
    vec2 local = vec2(v.local_offset[0],v.local_offset[1]);
   
    float angle = objects[v.o_id].angle;

    float c = cos(angle);
    float s = sin(angle);

    vec2 projected = vec2(local.x * c - local.y * s, local.x * s + local.y * c) + center;
    

    return vec3(
        projected.x,
        projected.y,
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

    texCoord = get_uv(gl_VertexID);
    lineColor = vec3(1.0,1.0,0.0);
}

