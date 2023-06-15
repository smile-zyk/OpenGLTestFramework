#pragma once

#include "GLCommon.h"
#include "Texture.h"

namespace OpenGLPlus
{
	class Texture
	{
	public:
		explicit Texture(GLenum type, const char* path);
		~Texture();
		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;

		void generate_mipmap();
		void bind(int unit);
		void set_wrap_mode(GLenum direction, GLenum mode);
		void set_border_color(float color[4]);
		void set_minification_filter(GLenum fliter);
		void set_magification_filter(GLenum fliter);

		int get_width() { return width; }
		int get_height() { return height; }
		int get_channels() { return channels; }
		GLuint get_id() { return id; }
	private:
		GLuint id;
		int width;
		int height;
		int channels;
	};
}
