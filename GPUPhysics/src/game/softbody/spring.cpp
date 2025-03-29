#include "spring.h"

Spring::Spring(unsigned int v_1,unsigned int v_2, float k,float d, std::vector<VertexData>* vertices) {
	resting_length = glm::distance(
		glm::vec3((*vertices)[v_1].position[0], (*vertices)[v_1].position[1], (*vertices)[v_1].position[2]),
		glm::vec3((*vertices)[v_2].position[0], (*vertices)[v_2].position[1], (*vertices)[v_2].position[2])
	);

	v1 = v_1;
	v2 = v_2;
	spring_constant = k;
	damping = d;
}
void Spring::add_spring(Spring spring,std::vector<unsigned int>* spring_indices, std::vector<Spring>* springs) {
	spring_indices->push_back(spring.v1);
	spring_indices->push_back(spring.v2);

	springs->push_back(spring);
}