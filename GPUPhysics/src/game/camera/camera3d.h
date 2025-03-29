#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../window/window.h"
#include "camera.h"
#pragma once
class Camera3D : Camera
{
public:
	Camera3D(glm::vec3 t, glm::vec3 e, float f, Window* w);
	float FOV;
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
};

