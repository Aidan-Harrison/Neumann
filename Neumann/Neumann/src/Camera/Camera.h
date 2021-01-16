#ifndef Camera_h
#define Camera_h

#include <glm/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "..\Renderer.h"

using namespace GlobalRend;

struct Camera {
	glm::vec3 m_CamPos = glm::vec3(1.0f), m_CamUp = glm::vec3(1.0f), m_CamFront = glm::vec3(1.0f);
	glm::mat4 m_View = glm::mat4(1.0f), m_Projection = glm::mat4(1.0f);
	float FOV = 45.0f;
	Camera();
	~Camera();
	bool firstMouse = true;
	float mouseX = screenWidth / 2, mouseY = screenHeight / 2;
	float lastX = screenWidth / 2, lastY = screenHeight / 2;
	float yaw = -90.0f, pitch = 0.0f, fov = 45.0f;

	void Transformation(glm::mat4 &view, glm::mat4 &projection);
	void Translate(glm::mat4 &view);
	void CalculateMovement(Camera &cam, float xOffset, float yOffset);
};

#endif