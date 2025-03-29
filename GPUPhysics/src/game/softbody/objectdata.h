#pragma once

struct ObjectData {
public:
	int o_id = 0;
	int start_index = 0;
	int end_index = 0;
	int start_indices_index = 0;
	int end_indices_index = 0;
	//non physical points
	int meta_points = 0;
	float point_mass = 1.0;
	float center[2];
	float r = 1.0;
	float g = 1.0;
	float b = 1.0;
	//min_x, max_x, min_y, max_y
	int bounding_box[4];
	//with respect to bounding_box

	//top left, top right, bottom left, bottom right
	int bounds[8];

	float angle = 0.0f;
	ObjectData(int object, float p);

};