#version 450 
#extension GL_ARB_shading_language_include : require
#extension GL_NV_shader_atomic_float : enable
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

uniform float match_factor;
uniform float damp_factor;

void main() {
    if(gl_GlobalInvocationID.x >= vertices.length()) {
        return;
    }
    if (vertices[gl_GlobalInvocationID.x].o_id != 0) {
        vertices[gl_GlobalInvocationID.x].velocity[1] -= 0.01;
    }
}