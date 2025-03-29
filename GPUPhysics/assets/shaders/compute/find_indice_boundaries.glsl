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

layout(binding = 3,std430) buffer ssbo3 {
    int indices[];
};

uniform float deltaTime;

int find_object_id(int indice) {
    for(int i = 0;i<objects.length();i++) {
        if (objects[i].start_index <= indice && objects[i].end_index >= indice) {
            return i;
        }
    }
    return -1;
}
void main() {
    int i = int(gl_GlobalInvocationID.x);
    if (i < indices.length()-1 && i > 0) {
        int end_object = find_object_id(indices[i]);
        int start_object = find_object_id(indices[i+1]);
        if (end_object != start_object) {
            //shift in object line
            atomicExchange(objects[end_object].end_indices_index,i);
            atomicExchange(objects[start_object].start_indices_index,i + 1);
        }
    }
    else if (i == 0) {
        atomicExchange(objects[0].start_indices_index, i);
    }
    else if (i == indices.length()-1) {
        atomicExchange(objects[objects.length() - 1].end_indices_index, i);
    }
}