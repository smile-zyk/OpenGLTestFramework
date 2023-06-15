#include "VertexArray.h"
#include <iostream>

OpenGLPlus::VertexArray::VertexArray()
{
	GLCall(glCreateVertexArrays(1, &id))
}

OpenGLPlus::VertexArray::~VertexArray()
{
	std::cout << "~VertexArray()" << std::endl;
	GLCall(glDeleteVertexArrays(1, &id))
}

void OpenGLPlus::VertexArray::bind_vertex_buffer(GLuint bindingindex, const Buffer& buffer, GLintptr offset, GLsizei stride)
{
	vertex_count = buffer.size / stride;
	GLCall(glVertexArrayVertexBuffer(id, bindingindex, buffer.id, offset, stride));
}

void OpenGLPlus::VertexArray::bind_element_buffer(const Buffer& buffer)
{
	element_size = buffer.size;
	GLCall(glVertexArrayElementBuffer(id, buffer.id));
}

void OpenGLPlus::VertexArray::enable_attrib(GLuint attribindex)
{
	GLCall(glEnableVertexArrayAttrib(id, attribindex));
}

void OpenGLPlus::VertexArray::disable_attrib(GLuint attribindex)
{
	GLCall(glDisableVertexArrayAttrib(id, attribindex));
}

void OpenGLPlus::VertexArray::bind_attrib(GLuint attribindex, GLuint bindingindex)
{
	GLCall(glVertexArrayAttribBinding(id, attribindex, bindingindex));
}

void OpenGLPlus::VertexArray::set_attrib(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
	GLCall(glVertexArrayAttribFormat(id, attribindex, size, type, normalized, relativeoffset));
}

void OpenGLPlus::VertexArray::bind() const
{
	GLCall(glBindVertexArray(id));
}

void OpenGLPlus::VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}