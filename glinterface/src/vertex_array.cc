#include "vertex_array.h"
#include "glcommon.h"

glinterface::VertexArray::VertexArray()
{
	GLCall(glCreateVertexArrays(1, &id))
}

glinterface::VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &id))
}

void glinterface::VertexArray::bind_vertex_buffer(GLuint bindingindex, const Buffer& buffer, GLintptr offset, GLsizei stride)
{
	vertex_count =  buffer.size / stride;
	GLCall(glVertexArrayVertexBuffer(id, bindingindex, buffer.id, offset, stride));
}

void glinterface::VertexArray::bind_element_buffer(const Buffer& buffer)
{
	element_size = buffer.size;
	GLCall(glVertexArrayElementBuffer(id, buffer.id));
}

void glinterface::VertexArray::enable_attrib(GLuint attribindex)
{
	GLCall(glEnableVertexArrayAttrib(id, attribindex));
}

void glinterface::VertexArray::disable_attrib(GLuint attribindex)
{
	GLCall(glDisableVertexArrayAttrib(id, attribindex));
}

void glinterface::VertexArray::bind_attrib(GLuint attribindex, GLuint bindingindex)
{
	GLCall(glVertexArrayAttribBinding(id, attribindex, bindingindex));
}

void glinterface::VertexArray::set_attrib(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
	if (type == GL_INT || type == GL_UNSIGNED_INT ||
		type == GL_SHORT || type == GL_UNSIGNED_SHORT ||
		type == GL_BYTE || type == GL_UNSIGNED_BYTE)
	{
		GLCall(glVertexArrayAttribIFormat(id, attribindex, size, type, relativeoffset));
	}
	else if (type == GL_DOUBLE)
	{
		GLCall(glVertexArrayAttribLFormat(id, attribindex, size, type, relativeoffset));
	}
	else
	{
		GLCall(glVertexArrayAttribFormat(id, attribindex, size, type, normalized, relativeoffset));
	}
}

void glinterface::VertexArray::bind() const
{
	GLCall(glBindVertexArray(id));
}

void glinterface::VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}