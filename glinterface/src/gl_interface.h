#pragma once
#include "glcommon.h"
#include "shader_program.h"
#include "vertex_array.h"

namespace glinterface
{
	class GLInterface
	{
	public:
		void set_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
		void clear(GLbitfield mask);
		void enable(GLenum cap);

		void draw_arrays(GLenum mode, const VertexArray& vao, const ShaderProgram& program);
		void draw_elements(GLenum mode, GLenum type, const VertexArray& vao, const ShaderProgram& program);

		void set_polygon_mode(GLenum face, GLenum mode);
	};
}

