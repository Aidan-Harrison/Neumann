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

void Shader::Use() { glUseProgram(m_ID); }

void Shader::SetBool(const std::string &name, bool value) const					{ GLCall(glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value))); }
void Shader::SetInt(const std::string &name, int value) const					{ GLCall(glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value)); }
void Shader::SetFloat(const std::string &name, float value) const				{ GLCall(glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value)); }
void Shader::SetVec2(const std::string& name, const glm::vec2& value) const     { GLCall(glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0])); }
void Shader::SetVec2(const std::string& name, float x, float y) const			{ GLCall(glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y)); }
void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const     { GLCall(glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0])); }
void Shader::SetVec3(const std::string& name, float x, float y, float z) const  { GLCall(glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z)); }
void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const		{ GLCall(glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0])); }
void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const		{ GLCall(glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0])); }
void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const		{ GLCall(glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0])); }