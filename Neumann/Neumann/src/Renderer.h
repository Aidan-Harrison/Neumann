#ifndef Renderer_h
#define Renderer_h

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

namespace GlobalRend { // Contains global variables
	extern const short screenWidth, screenHeight; // Check const!
	extern float deltaTime, lastFrame; // Delta of current and last frame
	extern bool firstMouse;
	extern double lastX, lastY;
};

// Make renderer class!

#endif