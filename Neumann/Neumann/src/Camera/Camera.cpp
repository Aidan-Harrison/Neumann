#include "Camera.h"

Camera::Camera() {
	m_CamPos =   glm::vec3(0.0f, 0.0f, 3.0f);
	m_CamUp =    glm::vec3(0.0f, 1.0f, 0.0f);
	m_CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
}

Camera::~Camera() {

}

void Camera::Transformation(glm::mat4 &view, glm::mat4 &projection) {
	view = glm::mat4(1.0f) = glm::lookAt(m_CamPos, m_CamPos + m_CamFront, m_CamUp);
	projection = glm::perspective(glm::radians(fov), static_cast<float>(800) / static_cast<float>(600), 0.1f, 100.0f);
}

void Camera::Translate(glm::mat4 &view) {
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
}

void Camera::CalculateMovement(Camera &cam, float xOffset, float yOffset) {
	cam.yaw += xOffset;
	cam.pitch += yOffset;

		// Clamp
	if (cam.pitch > 89.0f)
		cam.pitch = 89.0f;
	if (cam.pitch < -89.0f)
		cam.pitch = -89.0f;

		// Movement
	glm::vec3 direction = glm::vec3(0.0f);
	direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
	direction.y = sin(glm::radians(cam.pitch));
	direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
	cam.m_CamFront = glm::normalize(direction);
}