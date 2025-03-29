#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
class Window
{
private:
    GLFWwindow* window;
    int height;
    int width;

private:

public:
    Window(int width,int height,const char* name);
    int initialize();
    void processInput();
    
    GLFWwindow* getWindow();
    int getHeight();
    int getWidth();
};

