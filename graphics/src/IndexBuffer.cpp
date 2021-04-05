#include "IndexBuffer.h"
#include <glad/glad.h>
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count):_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GL_CALL(glGenBuffers(1, &_rendererID));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &_rendererID));
}

void IndexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));
}

void IndexBuffer::Unbind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}