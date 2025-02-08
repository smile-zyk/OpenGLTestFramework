#include "shader_program.h"

#include <iostream>

using namespace glinterface;

ShaderProgram::ShaderProgram()
{
	GLCall(id = glCreateProgram());
}

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	GLCall(id = glCreateProgram());
	attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, vertexPath));
	attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, fragmentPath));
	if (geometryPath != nullptr)
		attach_shader(std::make_shared<Shader>(GL_GEOMETRY_SHADER, geometryPath));
	link();
}

ShaderProgram::~ShaderProgram()
{
	GLCall(glDeleteProgram(id));
}

void ShaderProgram::link()
{
	GLCall(glLinkProgram(id));
	int success;
	char infoLog[512];
	GLCall(glGetProgramiv(id, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLCall(glGetProgramInfoLog(id, 1024, NULL, infoLog));
		std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" 
			<< infoLog << "\n" << std::endl;
	}
}

void ShaderProgram::use() const
{
	GLCall(glUseProgram(id));
}

void ShaderProgram::attach_shader(std::shared_ptr<Shader> shader)
{
	GLCall(glAttachShader(id, shader->id));
}

void ShaderProgram::set_uniform_value(const std::string& name, bool value) const
{
	GLCall(glUniform1i(getLocation(name), static_cast<int>(value)));
}

void ShaderProgram::set_uniform_value(const std::string& name, int value) const
{
	GLCall(glUniform1i(getLocation(name), value));
}

void ShaderProgram::set_uniform_value(const std::string& name, float value) const
{
	GLCall(glUniform1f(getLocation(name), value));
}

void ShaderProgram::set_uniform_value(const std::string& name, const glm::vec2& value) const
{
	GLCall(glUniform2fv(getLocation(name), 1, &value[0]));
}

void ShaderProgram::set_uniform_value(const std::string& name, float x, float y) const
{
	GLCall(glUniform2f(getLocation(name), x, y));
}

void ShaderProgram::set_uniform_value(const std::string& name, const glm::vec3& value) const
{
	GLCall(glUniform3fv(getLocation(name), 1, &value[0]));
}

void ShaderProgram::set_uniform_value(const std::string& name, float x, float y, float z) const
{
	GLCall(glUniform3f(getLocation(name), x, y, z));
}

void ShaderProgram::set_uniform_value(const std::string& name, const glm::vec4& value) const
{
	GLCall(glUniform4fv(getLocation(name), 1, &value[0]));
}

void ShaderProgram::set_uniform_value(const std::string& name, float x, float y, float z, float w) const
{
	GLCall(glUniform4f(getLocation(name), x, y, z, w));
}

void ShaderProgram::set_uniform_value(const std::string& name, const glm::mat2& mat) const
{
	GLCall(glUniformMatrix2fv(getLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void ShaderProgram::set_uniform_value(const std::string& name, const glm::mat3& mat) const
{
	GLCall(glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void ShaderProgram::set_uniform_value(const std::string& name, const glm::mat4& mat) const
{
	GLCall(glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void glinterface::ShaderProgram::set_patch_para(GLenum pname, GLint value)
{
	GLCall(glPatchParameteri(pname, value));
}

void glinterface::ShaderProgram::set_patch_para(GLenum pname, const GLfloat* values)
{
	GLCall(glPatchParameterfv(pname, values));
}

GLint ShaderProgram::getLocation(std::string name) const
{
	if (0 == locationCache.count(name))
	{
		GLCall(locationCache[name] = glGetUniformLocation(id, name.c_str()));
	}
	return locationCache[name];
}