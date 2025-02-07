#pragma once
#include <string>
#include <glcommon.h>

namespace glinterface
{
	class Shader
	{
	public:
		Shader(GLenum type, const char* path);
		~Shader();
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
	private:
		std::string path;
	protected:
		GLuint id;
		friend class ShaderProgram;
	};
}