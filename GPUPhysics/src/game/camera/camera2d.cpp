#include "camera2d.h"

Camera2D::Camera2D(glm::vec3 t, glm::vec3 e, float z, Window* w) {
	translation = t;
	euler_angles = e;
	window = w;
	zoom = z;

}

glm::mat4 Camera2D::getProjectionMatrix() {
	return glm::ortho(
		-window->getWidth() * zoom / 2.0f,
		window->getWidth() * zoom / 2.0f,
		-window->getHeight() * zoom / 2.0f,
		window->getHeight() * zoom / 2.0f,
	0.1f, 1000.0f);
}

glm::mat4 Camera2D::getViewMatrix() {
	glm::mat4 view = glm::translate(glm::mat4(1.0f),-translation);
	return view;
}

void Camera2D::update(float deltaTime) {
    GLFWwindow* gWindow = window->getWindow();

    double xpos, ypos;
    glfwGetCursorPos(gWindow, &xpos, &ypos);

    float forward = glfwGetKey(gWindow, GLFW_KEY_W) - glfwGetKey(gWindow, GLFW_KEY_S);
    float right = glfwGetKey(gWindow, GLFW_KEY_D) - glfwGetKey(gWindow, GLFW_KEY_A);
    float up = glfwGetKey(gWindow, GLFW_KEY_E) - glfwGetKey(gWindow, GLFW_KEY_Q);
	translation = translation + (forward * glm::vec3(0.0, 1.0, 0.0) + right * glm::vec3(1.0,0.0,0.0)) * speed * deltaTime * zoom;
	
	zoom += Input::yScrollOffset * zoom * 0.1;
	
	
}
