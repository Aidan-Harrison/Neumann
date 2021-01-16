#include "..\src\Shader\ShaderHandler.h"
#include "..\src\Camera\Camera.h"

// CONVERT TO HEADER AND CPP FILE FOR EASY CALLING

void LightMatSetup(Shader &shader, glm::vec3 lightPos, Camera &cam) {
	shader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	shader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

	shader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	// shader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
	shader.SetInt("material.texDiffuse", 0); // Texture
	shader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader.SetFloat("material.roughness", 32.0f);

	shader.SetVec3("objectColour", 1.0f, 0.5f, 0.31f);
	shader.SetVec3("lightColour", 1.0f, 1.0f, 1.0f);

	shader.SetVec3("lightPos", lightPos);
	shader.SetVec3("viewPos", cam.m_CamPos);
}

// Automate shader index as well 
void SetPointLight(Shader &shader, glm::vec3 lightPos[]) {
	shader.SetVec3("pLights[0].position", lightPos[0]);
	shader.SetVec3("pLights[0].ambient", 0.05f, 0.0f, 0.8f);
	shader.SetVec3("pLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	shader.SetVec3("pLights[0].specular", 1.0f, 1.0f, 1.0f);
	shader.SetFloat("pLights[0].constant", 1.0f);
	shader.SetFloat("pLights[0].linear", 0.09f);
	shader.SetFloat("pLights[0].quadratic", 0.032f);
}