#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window/window.h"
#include "shader/shader.h"
#include "../stb/stb_image.h"
#include "shader/compute.h"
#include "softbody/vertexdata.h"
#include <random>
#include "camera/camera2d.h"
#include "softbody/objectdata.h"
#include "softbody/shapes/square.h"
#include "softbody/shapes/circle.h"
#include "softbody/shapes/res_rect.h"
#include "softbody/spring.h"
#include "input/input.h"
#include "shader/shader_include.h"
class Game
{
private:
	Shader* point_shader;
	Shader* line_shader;
	Shader* center_shader;
	Shader* shape_match_shader;
	Shader* polygon_shader;
	Shader* triangle_shader;

	ComputeShader* computePhysics;
	ComputeShader* init_object_buffer;
	ComputeShader* computeSprings;
	ComputeShader* findCenters;
	ComputeShader* resetCenter;
	ComputeShader* findOffsets;
	ComputeShader* shapeMatching;
	ComputeShader* find_angles;
	ComputeShader* averageCenters;
	ComputeShader* averageAngles;
	ComputeShader* gravity;
	ComputeShader* integrate;
	ComputeShader* findBounds;
	ComputeShader* calculate_bounds;
	ComputeShader* init_indices_boundaries;
	ComputeShader* init_object_properties;

	int workgroup_size = 32;

	std::vector<VertexData> vertex_data;
	std::vector<ObjectData> object_data;
	std::vector<Spring> spring_data;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> spring_indices;
	std::vector<unsigned int> triangle_indices;

	GLuint vertexVAO;
	GLuint springVAO;
	GLuint triangleVAO;

	GLuint texture;

	GLuint vertexDataBuffer;
	GLuint objectDataBuffer;
	GLuint springsBuffer;
	GLuint indicesBuffer;

	double timestep = 0.01;
	double normal_timestep = 0.016;
	double slowed_timestep = 0.05;

	/*render modes
	1 NORMAL - edges and triangles 
	2 WIREFRAME - edges, springs and points 
	3 POINTS - points and center of mass
	4 VIRTUALFRAME - shape match and center of mass
	5 BOUNDING BOXES - bounding boxes
	*/
	int render_mode = 1;

	float offset = 0;
	float deltaTime;
	float currentTime;
	float lastTime;
	bool ticked = false;
	Window* window;
	Camera2D* camera;

	int object_id;
public:
	Game();
	int setup();
	int run();
	//update render_mode and slow motion
	void update_state();
	void draw();
	void tick(float timestep);
};

