#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../window/window.h"
#pragma once
class Camera
{
public:
	Window* window;
	glm::vec3 translation;
	//pitch, yaw
	glm::vec2 euler_angles;
	virtual glm::mat4 getViewMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix() = 0;
};

