#version 450
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

layout(local_size_x = 10,local_size_y = 1, local_size_z = 1) in;

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
    VertexData v = vertices[gl_GlobalInvocationID.x];
    vertices[gl_GlobalInvocationID.x].local_offset[0] = v.position[0] - objects[v.o_id].center[0];
    vertices[gl_GlobalInvocationID.x].local_offset[1] = v.position[1] - objects[v.o_id].center[1];
    if (v.angle_calculator == 0) {
        atomicAdd(objects[v.o_id].meta_points,1);
    }
}