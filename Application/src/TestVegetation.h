#pragma once
#include <vector>

#include <glm/vec3.hpp>
#include "TestBase.h"

namespace Test
{
	class TestVegetation : public TestBase
	{
	public:
		TestVegetation();
		~TestVegetation() override;
		virtual void OnUpdate(double deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnMouseClick(int button, int action, int mods) override;
		virtual void OnMouseMove(double x, double y) override;
		virtual void OnMouseScroll(double x, double y) override;
		virtual void OnKeyPress(int key, int scancode, int action, int mods) override;
	private:
		std::vector<glm::vec3> create_grass_vertices();
		std::vector<glm::vec3> grass_vertices;

		glinterface::ShaderProgram plane_program;
		glinterface::Texture plane_texture;
		glinterface::VertexArray plane_vertex_array;
		glinterface::Buffer plane_vertex_buffer;
		glinterface::ShaderProgram grass_program;
		glinterface::VertexArray grass_vertex_array;
		glinterface::Buffer grass_vertex_buffer;
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

