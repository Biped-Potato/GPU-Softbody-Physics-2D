#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
class Input {
public:
	static void init(GLFWwindow* window);
	static double xScrollOffset;
	static double yScrollOffset;
	
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
