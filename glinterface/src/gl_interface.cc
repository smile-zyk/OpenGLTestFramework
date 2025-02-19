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

void GLInterface::draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
	GLCall(glDrawElements(mode, count, type, indices));
}

void GLInterface::set_polygon_mode(GLenum face, GLenum mode)
{
	GLCall(glPolygonMode(face, mode));
}
