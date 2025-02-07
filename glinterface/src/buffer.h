#pragma once
#include "glcommon.h"

namespace glinterface
{
	class Buffer
	{
	public:
		Buffer();
		~Buffer();
		Buffer(const Buffer&) = delete;
		Buffer(Buffer&&) = delete;
		void allocate_storage(GLsizeiptr size, const void* data, GLbitfield flags);
		void allocate_data(GLsizeiptr size, const void* data, GLenum usage);
		void allocate_sub_data(GLintptr offset, GLsizeiptr size, const void* data);
		void* map_memory(GLenum access);
		void* map_memory_range(GLintptr offset, GLsizeiptr length, GLbitfield access);
		void unmap_memory();
	protected:
		GLuint id;
		friend class VertexArray;
		GLsizei size{};
	};
}