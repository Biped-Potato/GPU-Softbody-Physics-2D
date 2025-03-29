#version 450 
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"
layout (local_size_x = 10, local_size_y = 1, local_size_z = 1) in;

layout(binding = 0, std430) buffer ssbo {
    VertexData vertices[];
};

layout(binding = 1, std430) buffer ssbo1 {
    ObjectData objects[];
};
uniform float deltaTime;


void main() {
    if (gl_GlobalInvocationID.x < vertices.length()-1 && gl_GlobalInvocationID.x > 0) {
        int end_object = vertices[gl_GlobalInvocationID.x].o_id;
        int start_object = vertices[gl_GlobalInvocationID.x+1].o_id;
        if (end_object != start_object) {
            //shift in object line
            atomicExchange(objects[end_object].end_index,int(gl_GlobalInvocationID.x));
            atomicExchange(objects[start_object].start_index,int(gl_GlobalInvocationID.x + 1));
        }
    }
    else if (gl_GlobalInvocationID.x == 0) {
        atomicExchange(objects[0].start_index,0);
    }
    else if (gl_GlobalInvocationID.x == vertices.length()-1) {
        atomicExchange(objects[vertices[gl_GlobalInvocationID.x].o_id].end_index,int(gl_GlobalInvocationID.x));
    }
}