#version 450 
#extension GL_ARB_shading_language_include : require
#extension GL_NV_shader_atomic_float : enable
#include "/common.glsl"

layout (local_size_x = 10, local_size_y = 1, local_size_z = 1) in;

layout(binding = 0, std430) buffer ssbo {
    VertexData vertices[];
};

layout(binding = 1, std430) buffer ssbo1 {
    ObjectData objects[];
};

layout(binding = 2, std430) buffer ssbo2 {
    Spring springs[];
};

void main() {
    if(gl_GlobalInvocationID.x >= vertices.length()) {
        return;
    }
    uint i = gl_GlobalInvocationID.x;
    uint o = vertices[i].o_id;
    atomicMin(objects[o].bounding_box[0],int(vertices[i].position[0]));
    atomicMax(objects[o].bounding_box[1],int(vertices[i].position[0]));
    atomicMin(objects[o].bounding_box[2],int(vertices[i].position[1]));
    atomicMax(objects[o].bounding_box[3],int(vertices[i].position[1]));
}