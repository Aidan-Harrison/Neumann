#include <iostream>
#include "Renderer.h"

void GLClearError() {
	while (glGetError() != GL_NO_ERROR); // Error exists, clear it
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cerr << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << '\n';
		return false;
	}
	return true;
}

namespace GlobalRend {
	const short screenWidth = 800, screenHeight = 600; // Check const!
	float deltaTime = 0.0f, lastFrame = 0.0f; // Delta of current and last frame
	bool firstMouse = false;
	double lastX, lastY;
};