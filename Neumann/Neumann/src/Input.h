#ifndef Input_h
#define Input_h

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Renderer.h"

struct Input {
	void MouseCall(GLFWwindow *window, Camera &cam, double xPos, double yPos); // Capitalize
	void ScrollCall(GLFWwindow *window, Camera &cam, double xOffset, double yOffset);
	void ProccessInput(GLFWwindow *window, Camera &cam);
};

#endif