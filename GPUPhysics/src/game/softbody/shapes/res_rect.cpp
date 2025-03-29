#include "res_rect.h"

ResolutionRectangle::ResolutionRectangle(
	glm::vec3 _center,
	float _edge_length,
	int h_w,
	int h_h,
	int object,
	float p,
	float k,
	float d
) {
	edge_length = _edge_length;
	half_width = h_w;
	half_height = h_h;
	center = _center;
	o_id = object;
	point_mass = p;
	spring_constant = k;
	damping = d;
}

void ResolutionRectangle::insert(
	std::vector<VertexData>* vertices,
	std::vector<ObjectData>* objects,
	std::vector<unsigned int>* indices,
	std::vector<unsigned int>* triangle_indices,
	std::vector<unsigned int>* spring_indices,
	std::vector<Spring>* springs
) {
	//spawn points
	unsigned int start_indices = vertices->size();
	int height = half_height * 2 + 1;
	int width = half_width * 2 + 1;
	for (int x = -half_width; x <= half_width; x++) {
		for (int y = -half_height; y <= half_height; y++) {
			glm::vec3 position = center + glm::vec3(
				x, y, 0.0
			) * edge_length;
			vertices->push_back(
				VertexData(position, glm::vec3(0., 0., 0.), glm::vec2(0., 0.), o_id, 1)
			);
		}
	}
	objects->push_back(
		ObjectData(o_id, point_mass)
	);

	//vertical springs
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height - 1; y++) {
			int index = x * height + y;
			int next_index = index + 1;
			if (x == 0 || x == width - 1) {
				indices->push_back(start_indices + index);
				indices->push_back(start_indices + next_index);
			}
			Spring::add_spring(
				Spring(
					start_indices + index,
					start_indices + next_index,
					spring_constant,
					damping,
					vertices
				), spring_indices, springs
			);
		}
	}
	//horizontal springs
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width - 1; x++) {
			int index = x * height + y;
			int next_index = index + height;
			if (y == 0 || y == height - 1) {
				indices->push_back(start_indices + index);
				indices->push_back(start_indices + next_index);
			}
			Spring::add_spring(
				Spring(
					start_indices + index,
					start_indices + next_index,
					spring_constant,
					damping,
					vertices
				), spring_indices, springs
			);
		}
	}
	//diagonal springs
	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {
			int index = x * height + y;
			int next_index = index + height + 1;
			triangle_indices->push_back(start_indices + index);
			triangle_indices->push_back(start_indices + next_index);
			triangle_indices->push_back(start_indices + next_index - 1);

			Spring::add_spring(
				Spring(
					start_indices + index,
					start_indices + next_index,
					spring_constant,
					damping,
					vertices
				), spring_indices, springs
			);
		}
	}

	for (int y = 0; y < height - 1; y++) {
		for (int x = 1; x < width; x++) {
			int index = x * height + y;
			int next_index = index - height + 1;

			triangle_indices->push_back(start_indices + next_index);
			triangle_indices->push_back(start_indices + next_index + height);
			triangle_indices->push_back(start_indices + next_index - 1);

			Spring::add_spring(
				Spring(
					start_indices + index,
					start_indices + next_index,
					spring_constant,
					damping,
					vertices
				), spring_indices, springs
			);
		}
	}
}