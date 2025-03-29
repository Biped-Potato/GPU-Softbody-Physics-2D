#include "square.h"

Square::Square(glm::vec3 _center, float _edge_length,int object, float p, float k,float d) {
	center = _center;
	edge_length = _edge_length;
	o_id = object;
	point_mass = p;
	spring_constant = k;
	damping = d;
}

void Square::insert(
	std::vector<VertexData>* vertices, 
	std::vector<ObjectData>* objects, 
	std::vector<unsigned int>* indices,
	std::vector<unsigned int>* triangle_indices,
	std::vector<unsigned int>* spring_indices,
	std::vector<Spring> * springs
) {
	unsigned int start_indices = vertices->size();

	//top left, top right, bottom right, bottom left
	//0, 1, 2
	//2, 3, 0
	vertices->push_back(
		VertexData(center + glm::vec3(-edge_length, edge_length, 0.),glm::vec3(0.,0.,0.),glm::vec2(0.,0.),o_id, 1)
	);

	vertices->push_back(
		VertexData(center + glm::vec3(edge_length, edge_length, 0.), glm::vec3(0., 0., 0.), glm::vec2(0., 0.), o_id, 1)
	);

	vertices->push_back(
		VertexData(center + glm::vec3(edge_length, -edge_length, 0.), glm::vec3(0., 0., 0.), glm::vec2(0., 0.), o_id, 1)
	);

	vertices->push_back(
		VertexData(center + glm::vec3(-edge_length, -edge_length, 0.), glm::vec3(0., 0., 0.), glm::vec2(0., 0.), o_id, 1)
	);

	objects->push_back(
		ObjectData(o_id,point_mass)
	);

	for (int i = 0; i < 3; i++) {
		indices->push_back(start_indices + i);
		indices->push_back(start_indices + i + 1);

		

		Spring::add_spring(Spring(start_indices + i, start_indices + i + 1, spring_constant,damping, vertices), spring_indices, springs);
	}

	indices->push_back(start_indices+3);
	indices->push_back(start_indices);

	triangle_indices->push_back(start_indices);
	triangle_indices->push_back(start_indices + 1);
	triangle_indices->push_back(start_indices + 2);

	triangle_indices->push_back(start_indices + 2);
	triangle_indices->push_back(start_indices + 3);
	triangle_indices->push_back(start_indices);

	Spring::add_spring(Spring(start_indices + 3, start_indices, spring_constant,damping, vertices),spring_indices,springs);

	//diagonal springs
	Spring::add_spring(Spring(start_indices, start_indices + 2, spring_constant,damping, vertices), spring_indices, springs);
	Spring::add_spring(Spring(start_indices + 1, start_indices + 3, spring_constant,damping, vertices), spring_indices, springs);


}