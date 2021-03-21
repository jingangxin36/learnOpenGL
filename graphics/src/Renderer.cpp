#include "Renderer.h"
#include <iostream>
#include "VertexArray.h"
#include <glad/glad.h>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, const int line)
{
	while (const GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawArray(const VertexArray& va, size_t count, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}