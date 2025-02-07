#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#define ASSERT(x)\
	if(!(x)) __debugbreak()
#define GLCall(x)\
	GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

namespace glinterface {
	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
}


