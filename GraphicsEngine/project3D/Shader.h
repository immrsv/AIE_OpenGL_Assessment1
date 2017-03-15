#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>

using namespace std;

class Shader
{
public:
	static Shader* CompileShaders(std::string name, std::string vsFile, std::string fsFile);
	static Shader* GetShader(std::string name);

	~Shader();

	void MakeActive();

	void SetMat4(string paramName, const float* data);
	void SetVec4(string paramName, const float* data);
	void SetVec3(string paramName, const float* data);
	void SetVec2(string paramName, const float* data);

	void SetMat4Array(string paramName, unsigned int arrayCount, const float* data);
	void SetVec4Array(string paramName, unsigned int arrayCount, const float* data);
	void SetVec3Array(string paramName, unsigned int arrayCount, const float* data);
	void SetFloatArray(string paramName, unsigned int arrayCount, const float* data);

	void SetTexture(string paramName, unsigned int textureSlot, unsigned int textureId);

	void SetBool(string paramName, bool data);
	void SetInt(string paramName, const int data);
	void SetFloat(string paramName, const float data);
	
private:
	static bool LoadShaderFromFile(const char* filePath, std::string& code);

	static std::map<std::string, Shader*> collection;

	Shader(unsigned int programId) : _programId(programId) {}
	unsigned int _programId;
};