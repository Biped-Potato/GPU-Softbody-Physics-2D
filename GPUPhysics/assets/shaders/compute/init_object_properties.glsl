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

    float seed = float(gl_GlobalInvocationID.x) * 3;
    objects[gl_GlobalInvocationID.x].r = fract(sin(GOLDEN * seed));
    objects[gl_GlobalInvocationID.x].g = fract(sin(GOLDEN * seed + 1));
    objects[gl_GlobalInvocationID.x].b = fract(sin(GOLDEN * seed + 2));
}