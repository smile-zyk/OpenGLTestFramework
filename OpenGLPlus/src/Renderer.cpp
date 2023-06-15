#include "Renderer.h"

using namespace OpenGLPlus;

void Renderer::set_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	GLCall(glClearColor(red, green, blue, alpha));
}

void Renderer::clear(GLbitfield mask)
{
	GLCall(glClear(mask));
}

void Renderer::enable(GLenum cap)
{
	GLCall(glEnable(cap));
}

void Renderer::draw_arrays(GLenum mode, const VertexArray& vao, const ShaderProgram& program)
{
	vao.bind();
	program.use();
	GLCall(glDrawArrays(mode, 0, vao.get_vertex_count()));
}

void Renderer::draw_elements(GLenum mode, GLenum type, const VertexArray& vao, const ShaderProgram& program)
{
	vao.bind();
	program.use();
	int count = 0;
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

void Renderer::set_polygon_mode(GLenum face, GLenum mode)
{
	GLCall(glPolygonMode(face, mode));
}
