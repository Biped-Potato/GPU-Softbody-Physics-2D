#pragma once
#include "camera.h"
#include "../input/input.h"
class Camera2D : Camera
{
public:
	float speed = 300.0;
	Camera2D(glm::vec3 t, glm::vec3 e, float z, Window* w);
	float zoom = 1.0f;
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void update(float deltaTime);
};

