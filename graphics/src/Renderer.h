#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glad/glad.h>

#define ASSERT(x)\
	if(!(x)) __debugbreak();
#define GL_CALL(x)\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear() const;
	void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawArray(const VertexArray& va, size_t count, const Shader& shader) const;
};