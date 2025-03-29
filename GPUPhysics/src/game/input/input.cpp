#include "input.h"

void Input::init(GLFWwindow* window) {
	glfwSetScrollCallback(window, scrollCallback);
}
void Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	xScrollOffset = xScrollOffset - xoffset;
	yScrollOffset = yScrollOffset - yoffset;

}


double Input::xScrollOffset = 0;
double Input::yScrollOffset = 0;