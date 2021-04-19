#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string & name, const glm::vec3 v3);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

	unsigned int getRenderID() { return _rendererID; }
private:
	std::string _filePath;
	unsigned int _rendererID;
	std::unordered_map<std::string, int> _uniformLocationCache;
	ShaderProgramSource _parseShader(const std::string& filePath);
	unsigned int _compileShader(unsigned type, const std::string& source);
	unsigned int _createShader(const std::string& vertexShader, const std::string& fragmentShader);

	int _getUniformLocation(const std::string& name);

};
