#include "gl_interface.h"
#include "glcommon.h"

using namespace glinterface;

void GLInterface::set_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	GLCall(glClearColor(red, green, blue, alpha));
}

void GLInterface::clear(GLbitfield mask)
{
	GLCall(glClear(mask));
}

void GLInterface::enable(GLenum cap)
{
	GLCall(glEnable(cap));
}

void GLInterface::disable(GLenum cap)
{
	GLCall(glDisable(cap));
}

void GLInterface::draw_arrays(GLenum mode, GLint count)
{
	GLCall(glDrawArrays(mode, 0, count));
}

void GLInterface::draw_arrays(GLenum mode, const VertexArray& vao, const ShaderProgram& program)
{
	vao.bind();
	program.use();
	GLCall(glDrawArrays(mode, 0, vao.get_vertex_count()));
}

void GLInterface::draw_elements(GLenum mode, GLenum type, const VertexArray& vao, const ShaderProgram& program)
{
	vao.bind();
	program.use();
	GLsizei count = 0;
	switch (type)
	{
	case GL_UNSIGNED_BYTE:
		count = vao.get_element_size() / sizeof(unsigned char);
		break;
	case GL_UNSIGNED_SHORT:
		count = vao.get_element_size() / sizeof(unsigned short);
		break;
	case GL_UNSIGNED_INT:
		count = vao.get_element_size() / sizeof(unsigned int);
		break;
	default:
		break;
	}
	GLCall(glDrawElements(mode, count, type, 0));
}

void GLInterface::set_polygon_mode(GLenum face, GLenum mode)
{
	GLCall(glPolygonMode(face, mode));
}
