#pragma once
#include "GLCommon.h"
#include "Buffer.h"

namespace OpenGLWrapper
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		void bind_vertex_buffer(GLuint bindingindex, const Buffer& buffer, GLintptr offset, GLsizei stride);
		void bind_element_buffer(const Buffer& buffer);
		void bind_attrib(GLuint attribindex, GLuint bindingindex);
		void set_attrib(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
		void enable_attrib(GLuint attribindex);
		void disable_attrib(GLuint attribindex);
		void bind() const;
		void unbind() const;
		GLsizei get_vertex_count() const { return vertex_count; }
		GLsizei get_element_size() const { return element_size; }
	private:
		GLuint id;
		GLsizei vertex_count{};
		GLsizei element_size{};
	};
}