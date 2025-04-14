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

void main() {
    if(gl_GlobalInvocationID.x >= vertices.length()) {
        return;
    }
    uint i = gl_GlobalInvocationID.x;
    uint o = vertices[i].o_id;
    vec3 velocity = vec3(
        vertices[i].velocity[0],
        vertices[i].velocity[1],
        vertices[i].velocity[2]
    );

    if (length(velocity) > 2.5) {
        velocity = normalize(velocity) * 2.5;
    }
    vertices[i].velocity[0] = velocity.x;
    vertices[i].velocity[1] = velocity.y;
    vertices[i].velocity[2] = velocity.z;

    vertices[i].position[0] += velocity.x;
    vertices[i].position[1] += velocity.y;
    vertices[i].position[2] += velocity.z;
}