#pragma once
#include "GLCommon.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace OpenGLPlus
{
	class ShaderProgram
	{
	public:
		explicit ShaderProgram();
		explicit ShaderProgram(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		~ShaderProgram();
		ShaderProgram(const ShaderProgram&) = delete;
		void link();
		void use() const;
		void attach_shader(std::shared_ptr<Shader> shader);

		void set_uniform_value(const std::string& name, bool value) const;
		void set_uniform_value(const std::string& name, int value) const;
		void set_uniform_value(const std::string& name, float value) const;
		void set_uniform_value(const std::string& name, const glm::vec2& value) const;
		void set_uniform_value(const std::string& name, float x, float y) const;
		void set_uniform_value(const std::string& name, const glm::vec3& value) const;
		void set_uniform_value(const std::string& name, float x, float y, float z) const;
		void set_uniform_value(const std::string& name, const glm::vec4& value) const;
		void set_uniform_value(const std::string& name, float x, float y, float z, float w) const;
		void set_uniform_value(const std::string& name, const glm::mat2& value) const;
		void set_uniform_value(const std::string& name, const glm::mat3& value) const;
		void set_uniform_value(const std::string& name, const glm::mat4& value) const;

		void set_patch_para(GLenum pname, GLint value);
		void set_patch_para(GLenum pname, const GLfloat* values);

		int getId() { return id; }
	private:
		mutable std::unordered_map<std::string, GLint> locationCache;
		GLint getLocation(std::string name) const;
		GLuint id;
	};
}