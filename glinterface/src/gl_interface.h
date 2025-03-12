#pragma once
#include "glcommon.h"

namespace glinterface
{
	class GLInterface
	{
	public:
		void set_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
		void clear(GLbitfield mask);
		void enable(GLenum cap);
		void disable(GLenum cap);
		void draw_arrays(GLenum mode, GLint count);
		void draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices);
		void set_polygon_mode(GLenum face, GLenum mode);
	};
}

