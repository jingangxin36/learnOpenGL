#include "Shader.h"
#include <glad/glad.h>
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>


Shader::Shader(const std::string& filePath): _filePath(filePath), _rendererID(0)
{
	const ShaderProgramSource source = _parseShader(filePath);
	_rendererID = _createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(_rendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(_rendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(_getUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(_getUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(_getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
	GLCall(glUniformMatrix4fv(_getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


int Shader::_getUniformLocation(const std::string& name)
{
	GLCall(int location = glGetUniformLocation(_rendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!"<< std::endl;
	}
	return location;
}



ShaderProgramSource Shader::_parseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	std::stringstream mainstreams[2];
	ShaderType type = ShaderType::NONE;
	std::string line;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			mainstreams[static_cast<int>(type)] << line << '\n';
		}
	}
	return{ mainstreams[0].str(), mainstreams[1].str() };

}

unsigned int Shader::_compileShader(unsigned int type, const std::string& source)
{
	const unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		const auto message = static_cast<char*>(alloca(length * sizeof(char))); 
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::_createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = _compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = _compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	ASSERT(vs);
	ASSERT(fs);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;

}