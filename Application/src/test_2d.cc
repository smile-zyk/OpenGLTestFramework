#include "test_2d.h"
#include "glcommon.h"
#include "boundingbox.h"
#include "shape.h"
#include "test_base.h"
#include <cstddef>
#include <glm/fwd.hpp>
#include <imgui.h>
#include <memory>
#include <vector>

using namespace glinterface;

const int kLayer = 100;
const int kRectangleNumber = 100;
const int kCircleNumber = 100;
const int kVertexNumber = kRectangleNumber * 4 + kCircleNumber * 3;
const int kIndexNumber = kRectangleNumber * 6 + kCircleNumber * 3;

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
        shape_shader_.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "..\\..\\Application\\Resource\\Shaders\\shape.vert"));
        shape_shader_.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "..\\..\\Application\\Resource\\Shaders\\shape.frag"));        
        shape_shader_.link();

        GLbitfield map_flags = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT;
        shape_vertex_buffer_.allocate_storage(kVertexNumber * sizeof(Vertex), nullptr, map_flags);
        shape_index_buffer_.allocate_storage(kIndexNumber * sizeof(GLuint), nullptr, map_flags);
        shape_vertex_buffer_map_ = static_cast<Vertex*>(shape_vertex_buffer_.map_memory_range(0, kVertexNumber * sizeof(Vertex), map_flags));
        shape_index_buffer_map_ = static_cast<GLuint*>(shape_index_buffer_.map_memory_range(0, kIndexNumber * sizeof(GLuint), map_flags));
        current_shape_vertex_size = 0;
        current_shape_index_size = 0;

        shape_vertex_array_.bind_vertex_buffer(0, shape_vertex_buffer_, offsetof(Vertex, position), sizeof(Vertex));
        shape_vertex_array_.bind_vertex_buffer(1, shape_vertex_buffer_, offsetof(Vertex, color), sizeof(Vertex));
        shape_vertex_array_.bind_vertex_buffer(2, shape_vertex_buffer_, offsetof(Vertex, mode), sizeof(Vertex));
        shape_vertex_array_.bind_vertex_buffer(3, shape_vertex_buffer_, offsetof(Vertex, parameter), sizeof(Vertex));
        shape_vertex_array_.bind_element_buffer(shape_index_buffer_);

        shape_vertex_array_.set_attrib(0, 3, GL_FLOAT, false, 0);
        shape_vertex_array_.set_attrib(1, 4, GL_FLOAT, false, 0);
        shape_vertex_array_.set_attrib(2, 1, GL_INT, false, 0);
        shape_vertex_array_.set_attrib(3, 3, GL_FLOAT, false, 0);
        shape_vertex_array_.bind_attrib(0, 0);
        shape_vertex_array_.bind_attrib(1, 1);
        shape_vertex_array_.bind_attrib(2, 2);
        shape_vertex_array_.bind_attrib(3, 3);
        shape_vertex_array_.enable_attrib(0);
        shape_vertex_array_.enable_attrib(1);
        shape_vertex_array_.enable_attrib(2);
        shape_vertex_array_.enable_attrib(3);
        
        gl_interface_.set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);
        gl_interface_.enable(GL_DEPTH_TEST);
        gl_interface_.enable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        camera_.SetNearFar( -2 * kLayer, 2 * kLayer);
        CreateShapes();
        DrawShapes();
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
        RenderScene();
        RenderShapes();
        RenderSelectArea(select_area_);
    }
    
    const char* select_area_modes[] = {"NoOutline", "SolidOutline", "DashOutline"};
    
    void Test2D::OnImGuiRender()
    {
        glm::vec2 min = select_area_.GetMin();
        glm::vec2 max = select_area_.GetMax();
        ImGui::Text("Select Rect is min(%.2f, %.2f), max(%.2f, %.2f)", min.x, min.y, max.x, max.y);
        BoundingBox viewport = camera_.GetViewport();
        glm::vec2 viewport_min = viewport.GetMin();
        glm::vec2 viewport_max = viewport.GetMax();
        ImGui::Text("Viewport is (%.2f, %.2f), (%.2f, %.2f)", viewport_min.x, viewport_min.y, viewport_max.x, viewport_max.y);
        ImGui::BeginGroup();
        ImGui::Text("Grid");
        ImGui::Separator();
        ImGui::Checkbox("X Axis", &grid_shader_parameter_.draw_x_axis);
        ImGui::SameLine();
        ImGui::Checkbox("Y Axis", &grid_shader_parameter_.draw_y_axis);
        ImGui::DragFloat("Axes Width", &grid_shader_parameter_.axes_width, 0.1f, 2.f, 5.f);
        ImGui::EndGroup();
        ImGui::BeginGroup();
        ImGui::Text("Select Rect");
        ImGui::Separator();
        ImGui::Combo("Mode", &rect_shader_parameter_.mode, select_area_modes, IM_ARRAYSIZE(select_area_modes));
        ImGui::DragFloat("Dash Size", &rect_shader_parameter_.dash_size, 0.1f, 1.f, 20.f);
        ImGui::DragFloat("Gap Size", &rect_shader_parameter_.gap_size, 0.1f, 1.f, 10.f);
        ImGui::DragFloat("Outline Width", &rect_shader_parameter_.outline_width, 0.1f, 2.f, 5.f);
        ImGui::ColorEdit4("Outline Color", &rect_shader_parameter_.outline_color.x);
        ImGui::ColorEdit4("Filled Color", &rect_shader_parameter_.filled_color.x);
        ImGui::EndGroup();
    }
    
    void Test2D::OnWindowResize(int width, int height)
    {
        camera_.SetWindowSize(width, height);
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
                select_area_.Reset();
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
                    select_area_.p1 = {currentX / screen_width, 1 - currentY / screen_height};
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
                select_area_.p2 = {currentX / screen_width, 1 - currentY / screen_height};
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
    
    void Test2D::Draw(Shape* shape)
    {
        shape->Accept(this);
    }
    
    void Test2D::Draw(Rectangle* rect)
    {
        std::vector<Vertex> vertices;
        vertices.reserve(4);

        BoundingBox box = rect->GetBoundingBox();
        glm::vec2 min = box.GetMin();
        glm::vec2 max = box.GetMax();
        glm::vec4 color = rect->color();
        float layer = rect->layer();

        Vertex v1;
        v1.position = { min, layer };
        v1.color = color;
        v1.mode = kRectangle;

        Vertex v2;
        v2.position = { max.x, min.y, layer };
        v2.color = color;
        v2.mode = kRectangle;

        Vertex v3;
        v3.position = { max, layer };
        v3.color = color;
        v3.mode = kRectangle;

        Vertex v4;
        v4.position = { min.x, max.y, layer };
        v4.color = color;
        v4.mode = kRectangle;

        vertices.emplace_back(v1);
        vertices.emplace_back(v2);
        vertices.emplace_back(v3);
        vertices.emplace_back(v4);

        static GLuint rect_indices[6] = { 0, 1, 2, 0, 2, 3};
        std::vector<GLuint> indices;
        indices.resize(6);
        for(int i = 0; i < 6; i++) indices[i] = current_shape_vertex_size + rect_indices[i];

        memcpy(&shape_vertex_buffer_map_[current_shape_vertex_size], vertices.data(), 4 * sizeof(Vertex));
        memcpy(&shape_index_buffer_map_[current_shape_index_size], indices.data(), 6 * sizeof(GLuint));
    
        current_shape_vertex_size += 4;
        current_shape_index_size += 6;
    }
    
    void Test2D::Draw(Circle* circle)
    {
        std::vector<Vertex> vertices;
        vertices.reserve(3);

        glm::vec2 center = circle->center();
        glm::vec4 color = circle->color();
        float radius = circle->radius();
        float layer = circle->layer();
        glm::vec3 pos{center, layer};

        Vertex v1;
        v1.position = pos;
        v1.color = color;
        v1.mode = kCircle;
        v1.parameter.x = 1.0f;
        v1.parameter.y = radius;

        Vertex v2;
        v2.position = pos;
        v2.color = color;
        v2.mode = kCircle;
        v2.parameter.x = 2.0f;
        v2.parameter.y = radius;

        Vertex v3;
        v3.position = pos;
        v3.color = color;
        v3.mode = kCircle;
        v3.parameter.x = 3.0f;
        v3.parameter.y = radius;

        vertices.emplace_back(v1);
        vertices.emplace_back(v2);
        vertices.emplace_back(v3);

        std::vector<GLuint> indices;
        indices.resize(3);
        for(int i = 0; i < 3; i++) indices[i] = current_shape_vertex_size + i;

        memcpy(&shape_vertex_buffer_map_[current_shape_vertex_size], vertices.data(), 3 * sizeof(Vertex));
        memcpy(&shape_index_buffer_map_[current_shape_index_size], indices.data(), 3 * sizeof(GLuint));
    
        current_shape_vertex_size += 3;
        current_shape_index_size += 3;
    }
    
    void Test2D::DrawShapes()
    {
        for(int i = 0; i < shape_list_.size(); i++)
            Draw(shape_list_[i].get());
    }
    
    void Test2D::RenderScene()
    {
        grid_shader_.use();
        grid_shader_.set_uniform_value("view_matrix", camera_.view_matrix());
        grid_shader_.set_uniform_value("projection_matrix", camera_.projection_matrix());
        grid_shader_.set_uniform_value("draw_x_axis", grid_shader_parameter_.draw_x_axis);
        grid_shader_.set_uniform_value("draw_y_axis", grid_shader_parameter_.draw_y_axis);
        grid_shader_.set_uniform_value("axes_width", grid_shader_parameter_.axes_width);
        grid_vertex_array_.bind();
        gl_interface_.draw_arrays(GL_TRIANGLES, 6);
    }
    
    void Test2D::RenderSelectArea(const BoundingBox& select_area)
    {
        if(select_area.IsValid())
        {
            rect_shader_.use();
            rect_shader_.set_uniform_value("view_matrix", camera_.view_matrix());
            rect_shader_.set_uniform_value("projection_matrix", camera_.projection_matrix());
            rect_shader_.set_uniform_value("rect_min", select_area.GetMin());
            rect_shader_.set_uniform_value("rect_max", select_area.GetMax());
            rect_shader_.set_uniform_value("mode", rect_shader_parameter_.mode);
            rect_shader_.set_uniform_value("dash_size", rect_shader_parameter_.dash_size);
            rect_shader_.set_uniform_value("gap_size", rect_shader_parameter_.gap_size);
            rect_shader_.set_uniform_value("outline_width", rect_shader_parameter_.outline_width);
            rect_shader_.set_uniform_value("outline_color", rect_shader_parameter_.outline_color);
            rect_shader_.set_uniform_value("filled_color", rect_shader_parameter_.filled_color);
            rect_vertex_array_.bind();
            gl_interface_.draw_arrays(GL_TRIANGLES, 6);
        }
    }
    
    void Test2D::RenderShapes()
    {
        shape_vertex_array_.bind();
        shape_shader_.use();
        shape_shader_.set_uniform_value("view_matrix", camera_.view_matrix());
        shape_shader_.set_uniform_value("projection_matrix", camera_.projection_matrix());
        gl_interface_.draw_elements(GL_TRIANGLES, current_shape_index_size, GL_UNSIGNED_INT, nullptr);
    }
    
    void Test2D::CreateShapes()
    {
        auto rect = std::make_unique<Rectangle>(glm::vec2{-100, -100}, glm::vec2{200, 200}, 0.f);
        rect->set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        shape_list_.push_back(std::move(rect));
        rect = std::make_unique<Rectangle>(glm::vec2{-50, -50}, glm::vec2{200, 200}, 20.f);
        rect->set_color(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        shape_list_.push_back(std::move(rect));
        auto circle = std::make_unique<Circle>(glm::vec2{-50, -50}, 100, 10.f);
        circle->set_color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        shape_list_.push_back(std::move(circle));
    }
}