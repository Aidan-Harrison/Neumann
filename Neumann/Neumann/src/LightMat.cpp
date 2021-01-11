#include "ShaderHandler.h"
#include "Camera.h"

void LightMatSetup(Shader &shader, glm::vec3 lightPos, Camera &cam) {
	shader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	shader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

	shader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	shader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
	shader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader.SetFloat("material.roughness", 32.0f);

	shader.SetVec3("objectColour", 1.0f, 0.5f, 0.31f);
	shader.SetVec3("lightColour", 1.0f, 1.0f, 1.0f);

	shader.SetVec3("lightPos", lightPos);
	shader.SetVec3("viewPos", cam.m_CamPos);
}