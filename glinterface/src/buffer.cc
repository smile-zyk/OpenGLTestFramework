#include "buffer.h"
#include <iostream>

using namespace glinterface;

Buffer::Buffer()
{
	GLCall(glCreateBuffers(1, &id));
}

Buffer::~Buffer()
{
	std::cout << "~Buffer()" << std::endl;
	GLCall(glDeleteBuffers(1, &id));
}

void Buffer::allocate_storage(GLsizeiptr size, const void* data, GLbitfield flags)
{
	this->size = static_cast<GLsizei>(size);
	GLCall(glNamedBufferStorage(id, size, data, flags));
}

void Buffer::allocate_data(GLsizeiptr size, const void* data, GLenum usage)
{
	this->size = static_cast<GLsizei>(size);
	GLCall(glNamedBufferData(id, size, data, usage));
}

void Buffer::allocate_sub_data(GLintptr offset, GLsizeiptr size, const void* data)
{
	GLCall(glNamedBufferSubData(id, offset, size, data));
}

void* Buffer::map_memory(GLenum access)
{
	GLCall(void* ret = glMapNamedBuffer(id, access));
	return ret;
}

void* Buffer::map_memory_range(GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	GLCall(void* ret = glMapNamedBufferRange(id, offset, length, access));
	return ret;
}

void Buffer::unmap_memory()
{
	GLCall(glUnmapNamedBuffer(id));
}