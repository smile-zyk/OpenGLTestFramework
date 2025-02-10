#include "test_2d.h"
#include "glcommon.h"
#include "rect.h"
#include "test_base.h"
#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <imgui.h>

using namespace glinterface;

namespace Test
{
    Test2D::Test2D(): camera_(screen_width, screen_height, -1.f, 1.f)
    {
        // init shaders
        grid_shader_.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "..\\..\\Application\\Resource\\Shaders\\grid.vert"));
        grid_shader_.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "..\\..\\Application\\Resource\\Shaders\\grid.frag"));
        grid_shader_.link();
        rect_shader_.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "..\\..\\Application\\Resource\\Shaders\\rect.vert"));
        rect_shader_.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "..\\..\\Application\\Resource\\Shaders\\rect.frag"));        
        rect_shader_.link();

        // bind select box vertex array
        GLbitfield map_flags = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT;
        rect_vertex_buffer_.allocate_storage(4 * sizeof(RectVertex), nullptr, map_flags);
        rect_vertex_buffer_map_ = rect_vertex_buffer_.map_memory_range(0, 4 * sizeof(RectVertex), map_flags);
        rect_index_buffer_.allocate_storage(6 * sizeof(GLuint), nullptr, map_flags);
        rect_index_buffer_map_ = rect_index_buffer_.map_memory_range(0, 6 * sizeof(GLuint), map_flags);
        rect_vertex_array_.bind_vertex_buffer(0, rect_vertex_buffer_, offsetof(RectVertex, screen_uv), sizeof(RectVertex));
        rect_vertex_array_.bind_vertex_buffer(1, rect_vertex_buffer_, offsetof(RectVertex, tex_coord), sizeof(RectVertex));
        rect_vertex_array_.set_attrib(0, 2, GL_FLOAT, false, 0);
        rect_vertex_array_.set_attrib(1, 2, GL_FLOAT, false, 0);
        rect_vertex_array_.bind_attrib(0, 0);
        rect_vertex_array_.bind_attrib(1, 1);
        rect_vertex_array_.enable_attrib(0);
        rect_vertex_array_.enable_attrib(1);
        rect_vertex_array_.bind_element_buffer(rect_index_buffer_);

        gl_interface_.set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);
        gl_interface_.enable(GL_DEPTH_TEST);
        gl_interface_.enable(GL_BLEND);
        gl_interface_.enable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    Test2D::~Test2D()
    {
        
    }
    
    void Test2D::OnUpdate(double deltaTime)
    {
        
    }

    void Test2D::OnRender()
    {
        gl_interface_.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        grid_shader_.use();
        grid_shader_.set_uniform_value("view_matrix", camera_.view_matrix());
        grid_shader_.set_uniform_value("projection_matrix", camera_.projection_matrix());
        grid_shader_.set_uniform_value("draw_x_axis", true);
        grid_shader_.set_uniform_value("draw_y_axis", true);
        grid_vertex_array_.bind();
        gl_interface_.draw_arrays(GL_TRIANGLES, 6);

        if(select_rect_.IsValid())
        {
            auto data = select_rect_.GetRenderData();
            memcpy(rect_vertex_buffer_map_, data.rect_vertices.data(), data.rect_vertices.size() * sizeof(RectVertex));
            memcpy(rect_index_buffer_map_, data.rect_indices.data(), data.rect_indices.size() * sizeof(GLuint));
            gl_interface_.draw_elements(GL_TRIANGLES, GL_UNSIGNED_INT, rect_vertex_array_, rect_shader_);
        }
    }
    
    void Test2D::OnImGuiRender()
    {
        glm::vec2 min = select_rect_.GetMin();
        glm::vec2 max = select_rect_.GetMax();
        ImGui::Text("rect is min(%.2f, %.2f), max(%.2f, %.2f)", min.x, min.y, max.x, max.y);
    }
    
    void Test2D::OnWindowResize(int width, int height)
    {
        camera_.SetViewPort(width, height);
    }

    void Test2D::OnMouseClick(int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                first_pressed = !mouse_left_pressed_ && !mouse_right_pressed_;
                mouse_right_pressed_ = true;
            }
            else if (action == GLFW_RELEASE)
            {
                mouse_right_pressed_ = false;
            }
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                first_pressed = !mouse_left_pressed_ && !mouse_right_pressed_;
                mouse_left_pressed_ = true;
            }
            else if (action == GLFW_RELEASE)
            {
                mouse_left_pressed_ = false;
                select_rect_.Reset();
            }
        }
    }
    
    void Test2D::OnMouseMove(double x, double y)
    {
        if (mouse_left_pressed_ || mouse_right_pressed_)
        {
            double currentX = x;
            double currentY = y;
            if (first_pressed)
            {
                origin_pressed_pos_ = { currentX, currentY };
                if(mouse_left_pressed_)
                    select_rect_.p1 = {currentX / screen_width, 1 - currentY / screen_height};
                first_pressed = false;
            }
            if(mouse_right_pressed_)
            {
                float xOffset = static_cast<float>(currentX - origin_pressed_pos_.x);
                float yOffset = static_cast<float>(currentY - origin_pressed_pos_.y);
                glm::vec2 mouseMotion = { xOffset / screen_width, -yOffset / screen_height };
                origin_pressed_pos_ = { currentX, currentY };
                camera_.Move(mouseMotion);
            }
            else if(mouse_left_pressed_)
            {
                select_rect_.p2 = {currentX / screen_width, 1 - currentY / screen_height};
            }
        }
    }
    
    void Test2D::OnMouseScroll(double x, double y)
    {
        camera_.Zoom(static_cast<float>(y));
    }
    
    void Test2D::OnKeyPress(int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            camera_.set_center({0.f, 0.f});
        }
    }
}