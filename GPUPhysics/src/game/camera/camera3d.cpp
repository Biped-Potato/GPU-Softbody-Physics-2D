#include "camera3d.h"

Camera3D::Camera3D(glm::vec3 t, glm::vec3 e, float f, Window* w) {
	translation = t;
	euler_angles = e;
	FOV = f;
	window = w;
}

glm::mat4 Camera3D::getProjectionMatrix() {
	return glm::perspective(glm::radians(FOV), (float)(window->getWidth() / window->getHeight()), 0.1f, 1000.0f);
}

glm::mat4 Camera3D::getViewMatrix() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(euler_angles.y)) * cos(glm::radians(euler_angles.x));
	direction.y = sin(glm::radians(euler_angles.x));
	direction.z = sin(glm::radians(euler_angles.y)) * cos(glm::radians(euler_angles.x));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(translation,
		glm::vec3(translation + direction),
		glm::vec3(0.0f, 1.0f, 0.0f));
	return view;
}