#pragma once

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Camera/Camera.h"
#include "TestBase.h"
#include "Renderer.h"
#include "Buffer.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"

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
		OpenGLWrapper::Renderer renderer;
	};
}