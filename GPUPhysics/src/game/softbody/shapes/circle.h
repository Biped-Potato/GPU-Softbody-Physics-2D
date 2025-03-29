#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "..\objectdata.h"
#include "..\vertexdata.h"
#include "..\spring.h"

constexpr double PI = 3.14159265358979323846;

class Circle {
public:
	float radius;
	glm::vec3 center;
	int o_id;
	float point_mass;
	float spring_constant;
	int point_count;
	float damping;
	Circle(glm::vec3 _center, float r, int object, float p, float k,float d, int p_count);

	void insert(
		std::vector<VertexData>* vertices,
		std::vector<ObjectData>* objects,
		std::vector<unsigned int>* indices,
		std::vector<unsigned int>* triangle_indices,
		std::vector<unsigned int>* spring_indices,
		std::vector<Spring>* springs
	);
};