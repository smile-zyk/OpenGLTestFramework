#include "VertexArray.h"
#include <iostream>

OpenGLWrapper::VertexArray::VertexArray()
{
	GLCall(glCreateVertexArrays(1, &id))
}

OpenGLWrapper::VertexArray::~VertexArray()
{
	std::cout << "~VertexArray()" << std::endl;
	GLCall(glDeleteVertexArrays(1, &id))
}

void OpenGLWrapper::VertexArray::bind_vertex_buffer(GLuint bindingindex, const Buffer& buffer, GLintptr offset, GLsizei stride)
{
	vertex_count =  buffer.size / stride;
	GLCall(glVertexArrayVertexBuffer(id, bindingindex, buffer.id, offset, stride));
}

void OpenGLWrapper::VertexArray::bind_element_buffer(const Buffer& buffer)
{
	element_size = buffer.size;
	GLCall(glVertexArrayElementBuffer(id, buffer.id));
}

void OpenGLWrapper::VertexArray::enable_attrib(GLuint attribindex)
{
	GLCall(glEnableVertexArrayAttrib(id, attribindex));
}

void OpenGLWrapper::VertexArray::disable_attrib(GLuint attribindex)
{
	GLCall(glDisableVertexArrayAttrib(id, attribindex));
}

void OpenGLWrapper::VertexArray::bind_attrib(GLuint attribindex, GLuint bindingindex)
{
	GLCall(glVertexArrayAttribBinding(id, attribindex, bindingindex));
}

void OpenGLWrapper::VertexArray::set_attrib(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
	GLCall(glVertexArrayAttribFormat(id, attribindex, size, type, normalized, relativeoffset));
}

void OpenGLWrapper::VertexArray::bind() const
{
	GLCall(glBindVertexArray(id));
}

void OpenGLWrapper::VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}