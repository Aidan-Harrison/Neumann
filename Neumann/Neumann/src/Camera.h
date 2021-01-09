#ifndef Camera_h
#define Camera_h

#include <glm/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Renderer.h"

struct Camera {
	glm::vec3 m_CamPos, m_CamUp, m_CamFront;
	glm::mat4 m_View, m_Projection;
	float FOV = 45.0f;
	Camera();
	~Camera();
	bool firstMouse = true;
	float mouseX = 800 / 2, mouseY = 600 / 2;
	float lastX = 800 / 2, lastY = 600 / 2;
	float yaw = -90.0f, pitch = 0.0f, fov = 45.0f;

	void Transformation(glm::mat4 &view, glm::mat4 &projection);
	void Translate(glm::mat4 &view);
};

#endif