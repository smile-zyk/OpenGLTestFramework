#pragma once

#include "gl_interface.h"
#include <GLFW/glfw3.h>

extern int screen_width;
extern int screen_height;

namespace Test
{
	class TestBase
	{
	public:
		TestBase() {}
		virtual ~TestBase() {}

		virtual void OnUpdate(double deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnMouseClick(int button, int action, int mods) {}
		virtual void OnMouseMove(double x, double y) {}
		virtual void OnMouseScroll(double x, double y) {}
		virtual void OnKeyPress(int key, int scancode, int action, int mods) {}
	protected:
		glinterface::GLInterface gl_interface_;
	};
}