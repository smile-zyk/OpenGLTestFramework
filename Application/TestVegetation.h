#pragma once
#include <vector>

#include <glm/vec3.hpp>

#include "Camera/Camera.h"
#include "TestBase.h"
#include "Renderer.h"
#include "Buffer.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Test
{
	class TestVegetation : public TestBase
	{
	public:
		TestVegetation();
		~TestVegetation() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnMouseClick(int button, int action, int mods) override;
		virtual void OnMouseMove(double x, double y) override;
		virtual void OnMouseScroll(double x, double y) override;
		virtual void OnKeyPress(int key, int scancode, int action, int mods) override;
	private:
		std::vector<glm::vec3> create_grass_vertices();
		std::vector<glm::vec3> grass_vertices;

		OpenGLPlus::ShaderProgram plane_program;
		OpenGLPlus::Texture plane_texture;
		OpenGLPlus::VertexArray plane_vertex_array;
		OpenGLPlus::Buffer plane_vertex_buffer;
		OpenGLPlus::ShaderProgram grass_program;
		OpenGLPlus::VertexArray grass_vertex_array;
		OpenGLPlus::Buffer grass_vertex_buffer;
		OpenGLPlus::Renderer renderer;
		Camera camera;

		glm::vec4 topColor = { 0.f, 0.8f, 0.f, 1.f };
		glm::vec4 btnColor = { 0.f, 0.2f, 0.f, 1.f };
		int layer = 5;
		float intensity = 20.f;
		float grassWidth = 0.03f;
		float grassHeight = 0.5f;
		bool isWireFrame = false;
		float windDirection = 0.f;
		float windIntensity = 30.f;
	};
}

