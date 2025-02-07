#include "glcommon.h"
#include <iostream>

using namespace glinterface;

void glinterface::GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool glinterface::GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}