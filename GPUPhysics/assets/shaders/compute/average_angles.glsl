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
    ObjectData o = objects[gl_GlobalInvocationID.x];

    objects[gl_GlobalInvocationID.x].angle = objects[gl_GlobalInvocationID.x].angle / float(o.end_index - o.start_index + 1 - o.meta_points); 
}