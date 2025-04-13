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

vec2 as_vec2(uint index) {
    return vec2(vertices[index].position[0],vertices[index].position[1]);
}
vec2 as_vec2_v(uint index) {
    return vec2(vertices[index].velocity[0],vertices[index].velocity[1]);
}
void main() {
    if(gl_GlobalInvocationID.x >= springs.length()) {
        return;
    }

    uint spring_index = gl_GlobalInvocationID.x;
    Spring spring = springs[spring_index];
    float spring_dist = distance(as_vec2(spring.v1),as_vec2(spring.v2));
    float compressed = spring_dist-spring.resting_length;

    vec2 direction = (as_vec2(spring.v2) - as_vec2(spring.v1)) / spring_dist;
    vec2 spring_force = direction * spring.spring_constant * compressed;
    vec2 damping = direction * dot(direction,as_vec2_v(spring.v2) - as_vec2_v(spring.v1)) * spring.damping;
    spring_force += damping;

    float mass_1 = objects[vertices[spring.v1].o_id].point_mass;
    float mass_2 = objects[vertices[spring.v2].o_id].point_mass;

    atomicAdd(vertices[spring.v1].velocity[0], spring_force.x/mass_1);
    atomicAdd(vertices[spring.v1].velocity[1], spring_force.y/mass_1);

    atomicAdd(vertices[spring.v2].velocity[0], -spring_force.x/mass_2);
    atomicAdd(vertices[spring.v2].velocity[1], -spring_force.y/mass_2);
}