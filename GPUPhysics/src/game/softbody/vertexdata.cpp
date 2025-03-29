#include "vertexdata.h"



VertexData::VertexData(glm::vec3 _position,glm::vec3 _velocity, glm::vec2 _uv,int object,int ac)
{
	position[0] = _position.x;
	position[1] = _position.y;
	position[2] = _position.z;

	velocity[0] = _velocity.x;
	velocity[1] = _velocity.y;
	velocity[2] = _velocity.z;

	uv[0] = _uv.x;
	uv[1] = _uv.y;
	o_id = object;

	angle_calculator = ac;
}
