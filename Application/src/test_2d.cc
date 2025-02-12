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

        // GLbitfield map_flags = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT;

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
            rect_shader_.use();
            rect_shader_.set_uniform_value("view_matrix", camera_.view_matrix());
            rect_shader_.set_uniform_value("projection_matrix", camera_.projection_matrix());
            rect_shader_.set_uniform_value("rect_min", select_rect_.GetMin());
            rect_shader_.set_uniform_value("rect_max", select_rect_.GetMax());
            rect_shader_.set_uniform_value("mode", 2);
            rect_vertex_array_.bind();
            gl_interface_.draw_arrays(GL_TRIANGLES, 6);
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