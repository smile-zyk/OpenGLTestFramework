#pragma once

#include "buffer.h"
#include "shader_program.h"
#include "test_base.h"
#include "camera_2d.h"
#include "rect.h"
#include "vertex_array.h"
#include <glm/fwd.hpp>

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
        struct GridShaderParameter
        {
            bool draw_x_axis{true};
            bool draw_y_axis{true};
            float axes_width{2.f};
        };

        enum RectMode
        {
            kNoOutline,
            kSolidOutline,
            kDashOutline
        };

        struct RectShaderParameter
        {
            int mode{kDashOutline};
            float dash_size{8.f};
            float gap_size{4.f};
            float outline_width{2.f};
            glm::vec4 outline_color{0.8f, 0.8f, 0.8f, 0.8f};
            glm::vec4 filled_color{0.6f, 0.6f, 0.6f, 0.5f};
        };

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
        GridShaderParameter grid_shader_parameter_;
        RectShaderParameter rect_shader_parameter_;
    };
}