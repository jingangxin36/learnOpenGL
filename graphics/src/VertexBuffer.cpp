#include "VertexBuffer.h"
#include <glad/glad.h>
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned size)
{
	GL_CALL(glGenBuffers(1, &_rendererID));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &_rendererID));
}

void VertexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
}

void VertexBuffer::Unbind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}