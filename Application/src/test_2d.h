#pragma once

#include "shader_program.h"
#include "test_base.h"
#include "camera_2d.h"

namespace Test
{
    class Test2D : public TestBase
    {
    public:
        Test2D();
        ~Test2D() override;
        void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
        void OnWindowResize(int width, int height) override;
		void OnMouseClick(int button, int action, int mods) override;
		void OnMouseMove(double x, double y) override;
		void OnMouseScroll(double x, double y) override;
		void OnKeyPress(int key, int scancode, int action, int mods) override;
    private:
        Camera2D camera_;
        glinterface::ShaderProgram grid_shader_;
        glinterface::VertexArray grid_vertex_array_;
        bool leftMousePress = false;
		bool rightMousePress = false;
		bool firstPress = true;
		glm::vec2 lastMousePos = { 0.f,0.f };
    };
}