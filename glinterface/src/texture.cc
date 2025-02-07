#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cmath>
#include <iostream>

using namespace glinterface;

Texture::Texture(GLenum type, const char* path)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	GLCall(glCreateTextures(type, 1, &id));
	if (GL_TEXTURE_2D == type)
	{
		GLuint internalFormat = 0;
		GLuint externalFormat = 0;
		if (3 == channels)
		{
			internalFormat = GL_RGB8;
			externalFormat = GL_RGB;
		}
		else if (4 == channels)
		{
			internalFormat = GL_RGBA8;
			externalFormat = GL_RGBA;
		}
		else if (1 == channels)
		{
			internalFormat = GL_R8;
			externalFormat = GL_RED;
		}
		int levels = static_cast<int>(std::log(std::fmax(width, height)) + 1);
		GLCall(glTextureStorage2D(id, levels, internalFormat, width, height));
		GLCall(glTextureSubImage2D(id, 0, 0, 0, width, height, externalFormat,
			GL_UNSIGNED_BYTE, data));
		for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
		{
			std::cout << err << std::endl;
		}
	}
	else if (GL_TEXTURE_3D == type)
	{
		// process 3D Texture
	}
	stbi_image_free(data);
}

glinterface::Texture::~Texture()
{
	std::cout << "~Texture()" << std::endl;
	GLCall(glDeleteTextures(1, &id));
}

void Texture::generate_mipmap()
{
	GLCall(glGenerateTextureMipmap(id));
}

void Texture::bind(int unit)
{
	GLCall(glBindTextureUnit(unit, id));
}

void Texture::set_wrap_mode(GLenum direction, GLenum mode)
{
	GLCall(glTextureParameteri(id, direction, mode));
}

void Texture::set_border_color(float color[4])
{
	GLCall(glTextureParameterfv(id, GL_TEXTURE_BORDER_COLOR, color));
}

void Texture::set_minification_filter(GLenum fliter)
{
	GLCall(glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, fliter));
}

void Texture::set_magification_filter(GLenum fliter)
{
	GLCall(glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, fliter));
}