#include "test_2d.h"
#include "glcommon.h"
#include "test_base.h"

using namespace glinterface;

namespace Test
{
    Test2D::Test2D(): camera_(screen_width, screen_height, -1.f, 1.f)
    {
        grid_shader_.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "..\\..\\Application\\Resource\\Shaders\\grid.vert"));
        grid_shader_.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "..\\..\\Application\\Resource\\Shaders\\grid.frag"));
        grid_shader_.link();
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
        grid_shader_.set_uniform_value("draw_axis", true);
        grid_shader_.set_uniform_value("scene_center", camera_.center());
        gl_interface_.disable(GL_CULL_FACE);
        grid_vertex_array_.bind();
        gl_interface_.draw_arrays(GL_TRIANGLES, 6);
        gl_interface_.enable(GL_CULL_FACE);
    }
    
    void Test2D::OnImGuiRender()
    {
        
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
                firstPress = !leftMousePress && !rightMousePress;
                rightMousePress = true;
            }
            else if (action == GLFW_RELEASE)
            {
                rightMousePress = false;
            }
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                firstPress = !leftMousePress && !rightMousePress;
                leftMousePress = true;
            }
            else if (action == GLFW_RELEASE)
            {
                leftMousePress = false;
            }
        }
    }
    
    void Test2D::OnMouseMove(double x, double y)
    {
        if (leftMousePress || rightMousePress)
        {
            double currentX = x;
            double currentY = y;
            if (firstPress)
            {
                lastMousePos = { currentX, currentY };
                firstPress = false;
            }
            float xOffset = static_cast<float>(currentX - lastMousePos.x);
            float yOffset = static_cast<float>(currentY - lastMousePos.y);
            glm::vec2 mouseMotion = { xOffset / screen_width, -yOffset / screen_height };
            lastMousePos = { currentX, currentY };
            camera_.Move(mouseMotion);
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