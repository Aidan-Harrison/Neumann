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