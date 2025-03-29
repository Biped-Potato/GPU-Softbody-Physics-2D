#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "..\objectdata.h"
#include "..\vertexdata.h"
#include "..\spring.h"

class Square {
public:
	float edge_length;
	glm::vec3 center;
	int o_id;
	float point_mass;
	float spring_constant;
	float damping;
	Square(glm::vec3 _center, float _edge_length,int object,float p,float k,float d);

	void insert(
		std::vector<VertexData>* vertices, 
		std::vector<ObjectData>* objects,
		std::vector<unsigned int>* indices,
		std::vector<unsigned int>* triangle_indices,
		std::vector<unsigned int>* spring_indices,
		std::vector<Spring>* springs
	);
};