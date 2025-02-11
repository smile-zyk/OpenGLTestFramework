#pragma once

#include "buffer.h"
#include "shader_program.h"
#include "test_base.h"
#include "camera_2d.h"
#include "rect.h"
#include "vertex_array.h"

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
        glinterface::ShaderProgram rect_shader_;
        glinterface::VertexArray grid_vertex_array_;
        glinterface::VertexArray rect_vertex_array_;
        void* rect_vertex_buffer_map_;
        bool mouse_left_pressed_ = false;
		bool mouse_right_pressed_ = false;
		bool first_pressed = true;
		glm::vec2 origin_pressed_pos_ = { 0.f,0.f };
        Rect select_rect_;
    };
}