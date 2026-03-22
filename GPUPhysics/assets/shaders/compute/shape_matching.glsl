#version 450 
#extension GL_NV_shader_atomic_float : enable
#include "common.glsl"

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

    VertexData v = vertices[gl_GlobalInvocationID.x];

    vec2 center = vec2(objects[v.o_id].center[0],objects[v.o_id].center[1]);
    vec2 global = vec2(v.position[0],v.position[1]) - center;
    vec2 local = vec2(v.local_offset[0],v.local_offset[1]);
   
    float angle = objects[v.o_id].angle;

    float c = cos(angle);
    float s = sin(angle);

    vec2 projected = vec2(local.x * c - local.y * s, local.x * s + local.y * c);

    vec2 delta = projected - global;
    float len = length(delta);

    if (len > 1e-6) {
        vec2 direction = delta / len;

        vec2 spring_force = direction * match_factor * len;

        float mass = objects[v.o_id].point_mass;

        atomicAdd(vertices[gl_GlobalInvocationID.x].velocity[0], spring_force.x / mass);
        atomicAdd(vertices[gl_GlobalInvocationID.x].velocity[1], spring_force.y / mass);
    }

}