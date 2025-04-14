#version 450 
#extension GL_ARB_shading_language_include : require
#include "/common.glsl"

layout (local_size_x = 32, local_size_y = 1, local_size_z = 1) in;

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
    uint i = gl_GlobalInvocationID.x;
    ObjectData o = objects[i];

    //top_left
    objects[i].bounds[0] = o.bounding_box[0];
    objects[i].bounds[1] = o.bounding_box[3];
    //top_right
    objects[i].bounds[2] = o.bounding_box[1];
    objects[i].bounds[3] = o.bounding_box[3];
    //bottom_left
    objects[i].bounds[4] = o.bounding_box[0];
    objects[i].bounds[5] = o.bounding_box[2];
    //bottom_right
    objects[i].bounds[6] = o.bounding_box[1];
    objects[i].bounds[7] = o.bounding_box[2];
}