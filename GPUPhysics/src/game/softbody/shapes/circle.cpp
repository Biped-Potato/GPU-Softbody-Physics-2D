#include "circle.h"

Circle::Circle(glm::vec3 _center, float r, int object, float p, float k, float d,int p_count) {
	center = _center;
	radius = r;
	o_id = object;
	point_mass = p;
	spring_constant = k;
	point_count = p_count;
	damping = d;
}

void Circle::insert(
	std::vector<VertexData>* vertices,
	std::vector<ObjectData>* objects,
	std::vector<unsigned int>* indices,
	std::vector<unsigned int>* triangle_indices,
	std::vector<unsigned int>* spring_indices,
	std::vector<Spring>* springs
) {
	unsigned int start_indices = vertices->size();

	objects->push_back(
		ObjectData(o_id, point_mass)
	);

	for (int i = 0; i < point_count; i++) {
		double degree = i * ((2.0 * PI) / ((double)point_count));
		glm::vec3 position = center + glm::vec3(cosf(degree), sinf(degree), 0.) * radius;
		vertices->push_back(
			VertexData(position, glm::vec3(0., 0., 0.), glm::vec2(0., 0.), o_id, 1)
		);
	}
	vertices->push_back(
		VertexData(center, glm::vec3(0., 0., 0.), glm::vec2(0., 0.), o_id, 0)
	);
	for (int i = 0; i < point_count; i++) {
		indices->push_back(start_indices + i);
		indices->push_back(start_indices + ((i + 1) % (point_count)));

		triangle_indices->push_back(start_indices + i);
		triangle_indices->push_back(start_indices + ((i + 1) % (point_count)));
		triangle_indices->push_back(start_indices + point_count);

		Spring::add_spring(Spring(start_indices + i, start_indices + point_count, spring_constant, damping, vertices), spring_indices, springs);

		Spring::add_spring(Spring(start_indices + i, start_indices + ((i + 1) % (point_count)), spring_constant, damping, vertices), spring_indices, springs);
	}
}