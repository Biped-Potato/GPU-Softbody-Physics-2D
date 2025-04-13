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

layout(binding = 3,std430) buffer ssbo3 {
    int indices[];
};

int check_edge(vec2 p, vec2 e1, vec2 e2) {
    if (e1.y > e2.y) {
        vec2 temp = e1;
        e1 = e2;
        e2 = temp;
    }

    if (p.y < e1.y || p.y > e2.y) {
        return 0;
    }

    float slope = (e2.y - e1.y) / (e2.x - e1.x);
    float x_on_edge = e1.x + (p.y - e1.y) / slope;

    return p.x <= x_on_edge ? 1 : 0;
}
vec2 component_distance(vec2 p, vec2 e1, vec2 e2) {
    if (e1.y > e2.y) {
        vec2 temp = e1;
        e1 = e2;
        e2 = temp;
    }
    vec2 edge = e2 - e1;
    vec2 toPoint = p - e1;
    float edgeLength = length(edge);
    
    float t = clamp(dot(toPoint, edge) / (edgeLength * edgeLength),0.0,1.0);
    vec2 projected = t * edge;
    //                  perpendicular      , projected
    return vec2(distance(toPoint,projected), projected);
}
vec2 as_vec2(uint index) {
    return vec2(vertices[index].position[0],vertices[index].position[1]);
}
vec2 to_vec2(float vector[3]) {
    return vec2(vector[0],vector[1]);
}
void replace_vec2(uint index,vec2 value) {
    vertices[index].position[0] = value.x;
    vertices[index].position[1] = value.y;
}

void change_velocity(uint index,vec2 value){
    atomicExchange(vertices[index].velocity[0],value.x);
    atomicExchange(vertices[index].velocity[1],value.y);
}

vec2 find_normal(vec2 p,vec2 e1,vec2 e2){
    if (e2.y < e1.y) {
        vec2 temp = e1;
        e1 = e2;
        e2 = temp;
    }
    vec2 edge = e2 - e1;
    vec2 point = p - e1;

    //let's assume n1 is correct
    vec2 n1 = vec2(-edge.y, edge.x);
    vec2 n2 = vec2(edge.y, -edge.x);
    if (dot(n2,point) < 0.0) {
        n1 = n2;
    }

    return normalize(n1);
}

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);
    if(gl_GlobalInvocationID.x >= vertices.length()) {
        return;
    }


    VertexData vertex = vertices[gl_GlobalInvocationID.x];
    vec2 vertex_pos = vec2(vertex.position[0],vertex.position[1]);

    int intersections = 0;
    int intersect = -1;
    float tolerance = 1e-9;
    vertices[gl_GlobalInvocationID.x].intersect = 0;
    if (vertex.angle_calculator == 1) {
        for(int i = 0;i<objects.length();i++) {
            if(objects[i].o_id != vertex.o_id){
                intersections = 0;
                for (int j = objects[i].start_indices_index;j<objects[i].end_indices_index;j+=2) {
                    uint e1_index = indices[j];
                    uint e2_index = indices[j+1];

                    vec2 e1 = vec2(vertices[e1_index].position[0],vertices[e1_index].position[1]);
                    vec2 e2 = vec2(vertices[e2_index].position[0],vertices[e2_index].position[1]);

                    int intersected = check_edge(vertex_pos, e1, e2);

                    if (intersected == 1) {
                        if (abs(vertex_pos.y - e1.y) < tolerance) {
                            if (vertex_pos.y - e2.y > 0.0) {
                                intersections += 1;
                            }
                        }
                        else if (abs(vertex_pos.y - e2.y) < tolerance) {
                            if (vertex_pos.y - e1.y > 0.0) {
                                intersections += 1;
                            }
                        }
                        else {
                            intersections += 1;
                        }
                    }
                }

                if (intersections % 2 == 1) {
                    vertices[gl_GlobalInvocationID.x].intersect = 1;
                    intersect = i;
                    break;
                }
            }
        }
    }

    vertex_pos = vec2(vertex.position[0],vertex.position[1]);
    
    if (intersect != -1){
        bool found_valid_edge = false;
        vec2 closest_dist = vec2(-1.0,0.0);
        uint closest_point = 0;
        uint other_point_index = 0;

        for (
            int i = objects[intersect].start_indices_index;
            i < objects[intersect].end_indices_index;
            i +=2
        ) {
            uint e1_index = indices[i];
            uint e2_index = indices[i+1];

            vec2 e1 = vec2(vertices[e1_index].position[0],vertices[e1_index].position[1]);
            vec2 e2 = vec2(vertices[e2_index].position[0],vertices[e2_index].position[1]);

            vec2 p_dist = component_distance(vertex_pos, e1, e2);
            
            if (p_dist.x < closest_dist.x || !found_valid_edge) {
                closest_dist = p_dist;
                closest_point = e1_index;
                other_point_index = e2_index;
                found_valid_edge = true;
            }
        }

        if (found_valid_edge) {
            vec2 closest_vec = as_vec2(closest_point);
            vec2 other_point = as_vec2(other_point_index);

            if (distance(closest_vec,vertex_pos) > distance(other_point,vertex_pos)) {
                vec2 temp = closest_vec;
                closest_vec = other_point;
                other_point = temp;
                uint t = closest_point;
                closest_point = other_point_index;
                other_point_index = t;
            }

            vec2 n1 = find_normal(vertex_pos, other_point, closest_vec);
   
            float edge_mass = objects[intersect].point_mass * 2.0;
            float point_mass = objects[vertex.o_id].point_mass;

            float mass_ratio = (point_mass) / (point_mass + edge_mass);
        

            float closeness_ratio = (closest_dist.y / length(closest_vec-other_point));

            if (closeness_ratio < 0.5) {
                closeness_ratio = 1.0 - closeness_ratio;
            }

            float close_displacement = closeness_ratio * (2 * mass_ratio);
            float far_displacement = (1.0 - closeness_ratio) * (2 * mass_ratio);
            //calculate displacement of vertices
            vec2 close_delta = close_displacement * closest_dist.x * n1;
            vec2 far_delta = far_displacement * closest_dist.x * n1;
            vec2 point_delta = ((closeness_ratio * (close_displacement - far_displacement)) 
                + (1.0 - close_displacement)) * closest_dist.x * n1;
            //displace vertices with atomic operations
            atomicAdd(vertices[gl_GlobalInvocationID.x].position[0], point_delta.x);
            atomicAdd(vertices[gl_GlobalInvocationID.x].position[1], point_delta.y);

            atomicAdd(vertices[closest_point].position[0], -close_delta.x);
            atomicAdd(vertices[closest_point].position[1], -close_delta.y);

            atomicAdd(vertices[other_point_index].position[0], -far_delta.x);
            atomicAdd(vertices[other_point_index].position[1], -far_delta.y);

            float restitution = 0.9;
            
            float edge_velocity_x = (vertices[other_point_index].velocity[0] + vertices[closest_point].velocity[0]) * 0.5;
            float edge_velocity_y = (vertices[other_point_index].velocity[1] + vertices[closest_point].velocity[1]) * 0.5;

            vec2 initial_edge_velocity = vec2(edge_velocity_x,edge_velocity_y); 
            vec2 initial_point_velocity = to_vec2(vertices[gl_GlobalInvocationID.x].velocity);

            //both formulas for velocity have the same first half of the equation
            vec2 common_equation = 
                (edge_mass * initial_edge_velocity + point_mass * initial_point_velocity)
                / (point_mass + edge_mass);

            vec2 final_point_velocity = common_equation 
            + (edge_mass * restitution 
                * (initial_edge_velocity - initial_point_velocity)) 
                / (point_mass + edge_mass
            ); 

            vec2 final_edge_velocity = common_equation 
            + (point_mass * restitution 
                * (initial_point_velocity - initial_edge_velocity)) 
                / (point_mass + edge_mass
            ); 

            //wrapper to atomically exchange velocities
            change_velocity(gl_GlobalInvocationID.x, final_point_velocity);
            change_velocity(other_point_index, final_edge_velocity);
            change_velocity(closest_point, final_edge_velocity);
        }
    }
}