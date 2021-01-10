#ifndef Input_h
#define Input_h

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Renderer.h"

struct Input {
	void ProccessInput(GLFWwindow *window, Camera &cam);
};

#endif