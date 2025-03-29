#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct VertexData
{
public:
	float position[3];
	float velocity[3];
	float uv[2];
	float local_offset[2];
	//flag for when our vertices have intersected with something
	int intersect = 0;
	int angle_calculator = 0;
	//object id used to identify which object a vertex belongs to
	int o_id;

	VertexData(glm::vec3 _position, glm::vec3 _velocity, glm::vec2 _uv,int object,int ac);
};

