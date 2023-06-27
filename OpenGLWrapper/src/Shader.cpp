#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace OpenGLWrapper;

Shader::Shader(GLenum type, const char* path)
{
	std::string vertexCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		vertexCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADERPROGRAM::SHADER::FILE_NOT_SUCCESFULLY_READ :\n" 
			<< e.what() << std::endl;
	}
	this->path = path;
	const char* shaderCodeStr = vertexCode.c_str();
	GLCall(id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &shaderCodeStr, NULL));
	GLCall(glCompileShader(id));
	// check for shader compile errors
	int success;
	char infoLog[512];
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(id, 512, NULL, infoLog));
		std::cout << "ERROR::SHADERPROGRAM::SHADER::COMPILATION_FAILED\n"
			<< "SHADER FILE PATH:" << path << '\n' << infoLog << std::endl;
	}
}

Shader::~Shader()
{
	std::cout << "~Shader()" << std::endl;
	GLCall(glDeleteShader(id));
}