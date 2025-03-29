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

layout(binding = 2, std430) buffer ssbo2 {
    Spring springs[];
};

void main() {
    if(gl_GlobalInvocationID.x >= objects.length()) {
        return;
    }


    objects[gl_GlobalInvocationID.x].center = float[2](0.0,0.0);
    objects[gl_GlobalInvocationID.x].angle = 0.0;
    VertexData v = vertices[objects[gl_GlobalInvocationID.x].start_index];

    objects[gl_GlobalInvocationID.x].bounding_box[0] = int(v.position[0]);
    objects[gl_GlobalInvocationID.x].bounding_box[1] = int(v.position[0]);
    objects[gl_GlobalInvocationID.x].bounding_box[2] = int(v.position[1]);
    objects[gl_GlobalInvocationID.x].bounding_box[3] = int(v.position[1]);
}