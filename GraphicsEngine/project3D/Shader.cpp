#include <gl_core_4_4.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Shader.h"
#include <map>
#include <glm/ext.hpp>

std::map<std::string, Shader*> Shader::collection = std::map<std::string, Shader*>();

bool Shader::LoadShaderFromFile(const char* filePath, std::string& code)
{
	code = "";

	// Read the Vertex Shader code from the file
	std::ifstream VertexShaderStream(filePath, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string line;
		while (getline(VertexShaderStream, line))
			code += line + "\n";
		VertexShaderStream.close();
		return true;
	}
	else {
		printf("Cannot open shader file %s.\n", filePath);
		return false;
	}
}

Shader* Shader::GetShader(std::string name)
{
	std::map<std::string, Shader*>::iterator iter = collection.find(name);
	if (iter != collection.end()) {
		return iter->second;
	}

	std::cerr << "Requested Shader Not Found: " << name << std::endl;
	return 0;
}



Shader* Shader::CompileShaders(std::string name, std::string vsFile, std::string fsFile)
{

	std::map<std::string, Shader*>::iterator iter = collection.find(name);
	if (iter != collection.end()) {
		return iter->second;
	}

	std::string vsCode, fsCode;
	if (!(LoadShaderFromFile(vsFile.c_str(), vsCode) && LoadShaderFromFile(fsFile.c_str(), fsCode)))
		printf("failed to load shaders");
	const char* vsSource = vsCode.c_str();
	const char* fsSource = fsCode.c_str();

	GLuint programID;
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	collection[name] = new Shader(programID);
	return collection[name];
}

Shader::~Shader() {
	glDeleteProgram(_programId);
}

void Shader::MakeActive()
{
	glUseProgram(_programId);
}

void Shader::SetMat4(string paramName, const float* data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniformMatrix4fv(loc, 1, false, data);
}

void Shader::SetVec4(string paramName, const float* data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniform4fv(loc, 1, data);
}

void Shader::SetVec3(string paramName, const float* data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniform3fv(loc, 1, data);
}

void Shader::SetVec2(string paramName, const float * data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniform2fv(loc, 1, data);
}

void Shader::SetMat4Array(string paramName, unsigned int arrayCount, const float* data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniformMatrix4fv(loc, arrayCount, false, data);
}

void Shader::SetVec4Array(string paramName, unsigned int arrayCount, const float* data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniform4fv(loc, arrayCount, data);
}

void Shader::SetVec3Array(string paramName, unsigned int arrayCount, const float* data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniform3fv(loc, arrayCount, data);
}

void Shader::SetFloatArray(string paramName, unsigned int arrayCount, const float* data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniform1fv(loc, arrayCount, data);
}

void Shader::SetTexture(string paramName, unsigned int texSlot, unsigned int texHandle) {
	// Bind the texture
	glActiveTexture(GL_TEXTURE0 + texSlot);
	glBindTexture(GL_TEXTURE_2D, texHandle);

	// tell the shader where it is 
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniform1i(loc, /*GL_TEXTURE0 + */ texSlot);
}

void Shader::SetBool(string paramName, bool data) {
	unsigned int loc = glGetUniformLocation(_programId, paramName.c_str());
	glUniform1i(loc, (int)data);
}