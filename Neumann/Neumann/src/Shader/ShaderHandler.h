#ifndef ShaderHandler_h
#define ShaderHandler_h
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "..\Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

struct Shader {
	unsigned int m_ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void Use();
		// Basics
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
		// Vectors
	void SetVec2(const std::string &name, const glm::vec2 &value) const;
	void SetVec2(const std::string &name, float x, float y) const;
	void SetVec3(const std::string &name, const glm::vec3 &value) const;
	void SetVec3(const std::string &name, float x, float y, float z) const;
		// Matrices
	void SetMat2(const std::string &name, const glm::mat2 &mat) const;
	void SetMat3(const std::string &name, const glm::mat3 &mat) const;
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif