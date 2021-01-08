#include "ShaderHandler.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	/// Read Shaders
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;
	// Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try { // If failed try the following
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
			// Read files buffer contents into streams
		vShaderStream << vShaderFile.rdbuf(); // 'rdbuf()' = Read buffer
		fShaderStream << fShaderFile.rdbuf(); // 'rdbuf()' = Read buffer
			// Close handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure exception) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	/// Compiler Shaders
	unsigned int vertex, fragment;
	int succcess;
	char infoLog[512];
		// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vertex, 1, &vShaderCode, nullptr));
	GLCall(glCompileShader(vertex));
		// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fragment, 1, &fShaderCode, nullptr));
	GLCall(glCompileShader(fragment));
		// Shader Program
	m_ID = glCreateProgram();
	GLCall(glAttachShader(m_ID, vertex));
	GLCall(glAttachShader(m_ID, fragment));
	GLCall(glLinkProgram(m_ID));
		// Delete Shaders after linking
	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));
}

void Shader::Use() {
	glUseProgram(m_ID);
}

void Shader::SetBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) {
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}