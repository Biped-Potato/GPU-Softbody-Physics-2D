#define PI 3.1415926535897932384626433832795
//golden ratio
#define GOLDEN 0.61803398875

float angle_between(vec2 e1, vec2 e2) {
    return atan(e1.x * e2.y - e1.y * e2.x, dot(e1, e2));
}
struct VertexData {
  float position[3];
  float velocity[3];
  float uv[2];
  float local_offset[2];
  int intersect;
  int angle_calculator;
  int o_id;
};

struct ObjectData {
    int o_id;
    int start_index;
    int end_index;
    int start_indices_index;
    int end_indices_index;
    int meta_points;
    float point_mass;
    float center[2];
    float r;
    float g;
    float b;
    //bounding box is not really important for the simulation
    int bounding_box[4];
    //bounding box coords
    int bounds[8];
    float angle;
};

struct Spring {
   float resting_length;
   float spring_constant;
   float damping;
   uint v1;
   uint v2;
};