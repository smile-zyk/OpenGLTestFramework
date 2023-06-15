#include "Buffer.h"
#include <iostream>

OpenGLPlus::Buffer::Buffer()
{
	GLCall(glCreateBuffers(1, &id));
}

OpenGLPlus::Buffer::~Buffer()
{
	std::cout << "~Buffer()" << std::endl;
	GLCall(glDeleteBuffers(1, &id));
}

void OpenGLPlus::Buffer::allocate_storage(GLsizeiptr size, const void* data, GLbitfield flags)
{
	this->size = size;
	GLCall(glNamedBufferStorage(id, size, data, flags));
}

void OpenGLPlus::Buffer::allocate_data(GLsizeiptr size, const void* data, GLenum usage)
{
	this->size = size;
	GLCall(glNamedBufferData(id, size, data, usage));
}

void OpenGLPlus::Buffer::allocate_sub_data(GLintptr offset, GLsizeiptr size, const void* data)
{
	GLCall(glNamedBufferSubData(id, offset, size, data));
}

void* OpenGLPlus::Buffer::map_memory(GLenum access)
{
	GLCall(void* ret = glMapNamedBuffer(id, access));
	return ret;
}

void* OpenGLPlus::Buffer::map_memory_range(GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	GLCall(void* ret = glMapNamedBufferRange(id, offset, length, access));
	return ret;
}

void OpenGLPlus::Buffer::unmap_memory()
{
	GLCall(glUnmapNamedBuffer(id));
}