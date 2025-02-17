#pragma once

#include "buffer.h"
#include "boundingbox.h"
#include "glcommon.h"
#include "shader_program.h"
#include "shape.h"
#include "test_base.h"
#include "camera_2d.h"
#include "boundingbox.h"
#include "vertex_array.h"

#include <memory>
#include <vector>

namespace Test
{
    class Test2D : public TestBase, ShapeRenderer
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

        struct Vertex
        {
            glm::vec3 position;
            glm::vec4 color;
            int mode;
        };

        struct RenderItem
        {
            int offset;
            std::vector<GLuint> indices;
        };

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

        void Draw(Shape* shape);

        void Draw(Rectangle* rect) override;

        void Draw(Circle* circle) override;

        void RenderShapes();

        void RenderScene();

        void RenderSelectArea(const BoundingBox& select_area);

        void* rect_vertex_buffer_map_;
        bool mouse_left_pressed_ = false;
		bool mouse_right_pressed_ = false;
		bool first_pressed = true;
		glm::vec2 origin_pressed_pos_ = { 0.f,0.f };
        std::vector<std::unique_ptr<Shape>> shape_list_;
        
        BoundingBox select_area_;
        GridShaderParameter grid_shader_parameter_;
        RectShaderParameter rect_shader_parameter_;
        Camera2D camera_;

        // opengl objects
        glinterface::ShaderProgram grid_shader_;
        glinterface::ShaderProgram rect_shader_;
        glinterface::VertexArray grid_vertex_array_;
        glinterface::VertexArray rect_vertex_array_;
        glinterface::VertexArray shape_vertex_array_;
        glinterface::Buffer shape_vertex_buffer_;
        glinterface::Buffer shape_index_buffer_;
        glinterface::Buffer shape_render_buffer_;
    };
}