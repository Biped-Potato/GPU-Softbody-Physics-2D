#pragma once
#include <vector>
#include "vertexdata.h"
#include <iostream>
struct Spring {
	float resting_length;
	float spring_constant;
	float damping;
	//the two vertices from which this spring is connected 
	uint32_t v1;
	uint32_t v2;

	Spring(unsigned int v_1, unsigned int v_2, float k, float d,std::vector<VertexData>* vertices);
	static void add_spring(Spring spring,std::vector<unsigned int> * spring_indices, std::vector<Spring> * springs);
};
