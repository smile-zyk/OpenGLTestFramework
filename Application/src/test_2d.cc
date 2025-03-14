#include "test_2d.h"
#include "glcommon.h"
#include "boundingbox.h"
#include "shape.h"
#include "shape_layer.h"
#include "shape_rtree.h"
#include "test_base.h"
#include "thread_pool.h"
#include "prof_timer.h"

#include <GLFW/glfw3.h>
#include <cstring>
#include <functional>
#include <future>
#include <imgui.h>

#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <utility>
#include <vector>

using namespace glinterface;

bool EnableThreadPoolCreate = true;
bool EnableRTreeCulling = true;
int Layer = 1;
const int kMaxLayer = 10000;
const int kRectanglePerLayer = 50;
const int kCirclePerLayer = 50;
const int kVertexNumber = kMaxLayer * kRectanglePerLayer * 4 + kMaxLayer * kCirclePerLayer * 3;
const int kIndexNumber = kMaxLayer * kRectanglePerLayer * 6 + kMaxLayer * kCirclePerLayer * 3;

namespace Test
{
    Test2D::Test2D(): camera_(screen_width, screen_height, -1.f, 1.f)
    {
        // init shaders
        grid_shader_.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "shaders/grid.vert"));
        grid_shader_.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shaders/grid.frag"));
        grid_shader_.link();
        rect_shader_.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "shaders/rect.vert"));
        rect_shader_.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shaders/rect.frag"));        
        rect_shader_.link();
        shape_shader_.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "shaders/shape.vert"));
        shape_shader_.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shaders/shape.frag"));        
        shape_shader_.link();

        // init buffer & vertex array
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
        shape_vertex_array_.bind_vertex_buffer(4, shape_vertex_buffer_, offsetof(Vertex, selected), sizeof(Vertex));
        shape_vertex_array_.bind_element_buffer(shape_index_buffer_);

        shape_vertex_array_.set_attrib(0, 3, GL_FLOAT, false, 0);
        shape_vertex_array_.set_attrib(1, 4, GL_FLOAT, false, 0);
        shape_vertex_array_.set_attrib(2, 1, GL_INT, false, 0);
        shape_vertex_array_.set_attrib(3, 3, GL_FLOAT, false, 0);
        shape_vertex_array_.set_attrib(4, 1, GL_INT, false, 0);
        shape_vertex_array_.bind_attrib(0, 0);
        shape_vertex_array_.bind_attrib(1, 1);
        shape_vertex_array_.bind_attrib(2, 2);
        shape_vertex_array_.bind_attrib(3, 3);
        shape_vertex_array_.bind_attrib(4, 4);
        shape_vertex_array_.enable_attrib(0);
        shape_vertex_array_.enable_attrib(1);
        shape_vertex_array_.enable_attrib(2);
        shape_vertex_array_.enable_attrib(3);
        shape_vertex_array_.enable_attrib(4);
        
        // init gl state
        gl_interface_.set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);
        gl_interface_.enable(GL_DEPTH_TEST);
        gl_interface_.enable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // set camera near and far
        camera_.SetNearFar( -2.f * Layer, 2.f * Layer);
        
        shape_layer_list_.reserve(kMaxLayer);
        // create shapes and push to shape_list_
        CreateRandomShapes();
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
    
    double search_time = 0;
    double draw_time = 0;

    void Test2D::OnImGuiRender()
    {
        glm::vec2 min = select_area_.GetMin();
        glm::vec2 max = select_area_.GetMax();
        ImGui::Text("Select Rect is min(%.2f, %.2f), max(%.2f, %.2f)", min.x, min.y, max.x, max.y);
        BoundingBox viewport = camera_.GetViewport();
        glm::vec2 viewport_min = viewport.GetMin();
        glm::vec2 viewport_max = viewport.GetMax();
        ImGui::Text("Viewport is (%.2f, %.2f), (%.2f, %.2f)", viewport_min.x, viewport_min.y, viewport_max.x, viewport_max.y);
        ImGui::Text("Viewport cover shape number: %u", current_viewport_shape);
        ImGui::Text("Viewport search time: %lf s", search_time);
        ImGui::Text("Viewport draw time: %lf s", draw_time);
        ImGui::Text("Layer:");
        ImGui::SameLine();
        if(ImGui::InputInt("##hiddenLabel", &Layer))
        {
            if(Layer > kMaxLayer) Layer = kMaxLayer;
            if(Layer < 1)  Layer = 1;
            camera_.SetNearFar( -2.f * Layer, 2.f * Layer);
            ClearShapes();
            CreateRandomShapes();
            DrawShapes();
        }
        if(ImGui::Checkbox("RTree Culling", &EnableRTreeCulling))
        {
            ClearIndices();
            if(!EnableRTreeCulling)
            for(const auto &layer : shape_layer_list_)
            {
                auto shapes = layer->GetRTree().GetShapes();
                for(const auto& shape: shapes)
                    DrawShapeCallback(shape);
            }
        }
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

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            ClearSelectShape();
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
    
        shape_render_item_map_.insert({rect->uuid(), RenderItem{current_shape_vertex_size, kRectangle}});

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
    
        shape_render_item_map_.insert({circle->uuid(), RenderItem{current_shape_vertex_size, kCircle}});

        current_shape_vertex_size += 3;
        current_shape_index_size += 3;
    }
    
    void Test2D::DrawShapes()
    {
        for(int i = 0; i < shape_list_.size(); i++)
            Draw(shape_list_[i].get());
    }
    
    void Test2D::ClearShapes()
    {
        select_shape_list_.clear();
        shape_layer_list_.clear();
        shape_list_.clear();
        memset(shape_vertex_buffer_map_, 0, kVertexNumber * sizeof(Vertex));
        memset(shape_index_buffer_map_, 0, kIndexNumber * sizeof(GLuint));
        current_shape_index_size = 0;
        current_shape_vertex_size = 0;
    }
    
    void Test2D::ClearIndices()
    {
        memset(shape_index_buffer_map_, 0, kIndexNumber * sizeof(GLuint));
        current_shape_index_size = 0;
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

            ClearSelectShape();
            auto s_min = select_area.GetMin();
            auto s_max = select_area.GetMax();
            auto viewport = camera_.GetViewport();
            auto v_min = viewport.GetMin();
            auto v_max = viewport.GetMax();
            BoundingBox box{v_min + s_min * (v_max - v_min), v_min + s_max * (v_max - v_min)};
            ShapeRTree::ShapeRTreeCallback select_callback = std::bind(&Test2D::SelectShapeCallback, this, std::placeholders::_1);
            for(const auto &layer : shape_layer_list_)
            layer->GetRTree().Search(box, select_callback);
        }
    }
    
    void Test2D::RenderShapes()
    {
        ProfTimer search_timer("CreateRandomShapes Time");
        if(EnableRTreeCulling)
        {
            ClearIndices();
            current_viewport_shape = 0;
            auto view_port = camera_.GetViewport();
            ShapeRTree::ShapeRTreeCallback shape_call_back = std::bind(&Test2D::DrawShapeCallback, this, std::placeholders::_1);
            for(const auto &layer : shape_layer_list_)
                layer->GetRTree().Search(view_port, shape_call_back);
        }
        search_time = search_timer.SinceStartSecs();
        ProfTimer draw_timer("CreateRandomShapes Time");
        shape_vertex_array_.bind();
        shape_shader_.use();
        shape_shader_.set_uniform_value("view_matrix", camera_.view_matrix());
        shape_shader_.set_uniform_value("projection_matrix", camera_.projection_matrix());
        shape_shader_.set_uniform_value("screen_size", glm::vec2{screen_width, screen_height});
        gl_interface_.draw_elements(GL_TRIANGLES, current_shape_index_size, GL_UNSIGNED_INT, nullptr);
        draw_time = draw_timer.SinceStartSecs();
    }
    
    float GenerateRandomValue(float min, float max)
    {
        thread_local std::mt19937 generator(std::random_device{}());
    
        std::uniform_real_distribution<float> distribution(min, max);

        return distribution(generator);
    }

    glm::vec4 GenerateRandomColor()
    {
        float r = GenerateRandomValue(0.f, 1.f);
        float g = GenerateRandomValue(0.f, 1.f);
        float b = GenerateRandomValue(0.f, 1.f);
        float a = 0.6f;
        return {r, g, b, a};
    }

    glm::vec2 GenerateRandomPosition(float min, float max)
    {
        float x = GenerateRandomValue(min, max);
        float y = GenerateRandomValue(min, max);
        return {x, y};
    }

    std::unique_ptr<Rectangle> MakeRandomRectangle(float layer)
    {
        auto rect = std::make_unique<Rectangle>(GenerateRandomPosition(-10.f * Layer, 10.f * Layer), GenerateRandomPosition(Layer / 4.f, Layer / 2.f), layer);
        rect->set_color(GenerateRandomColor());
        return rect;
    }

    std::unique_ptr<Circle> MakeRandomCircle(float layer)
    {
        auto circle = std::make_unique<Circle>(GenerateRandomPosition(-10.f * Layer, 10.f * Layer), GenerateRandomValue(Layer / 8.f, Layer / 4.f), layer);
        circle->set_color(GenerateRandomColor());
        return circle;
    }

    Test2D::MakeLayerReturn Test2D::MakeRandomLayer(int layer_id)
    {
        Test2D::MakeLayerReturn r;
        r.shape_layer = std::make_unique<ShapeLayer>(layer_id);
        float current_layer = r.shape_layer->depth();
        for(int j = 0; j < kRectanglePerLayer; j++)
        {
            auto rect = MakeRandomRectangle(current_layer);
            r.shape_layer->GetRTree().Insert(rect.get());
            r.shape_list.emplace_back(std::move(rect));
        }
        for(int j = 0; j < kCirclePerLayer; j++)
        {
            auto circle = MakeRandomCircle(current_layer);
            r.shape_layer->GetRTree().Insert(circle.get());
            r.shape_list.push_back(std::move(circle));
        }
        return r;
    }
    

    void Test2D::CreateRandomShapes()
    {
        ProfTimer timer("CreateRandomShapes Time");
        if(EnableThreadPoolCreate)
        {
            ThreadPool& thread_pool = GetThreadPool();
            int thread_number = thread_pool.get_thread_count();
            if(thread_number > Layer) thread_number = Layer;

            int layer_per_thread = Layer / thread_number;
            int remaining_layer = Layer % thread_number;

            typedef std::vector<MakeLayerReturn> MakeLayerReturnVector;
            typedef std::future<std::pair<int, MakeLayerReturnVector>> LayerVectorFuture;
            std::vector<LayerVectorFuture> future_list;

            int start = 0;
            int end = start;
            for(int i = 0; i < thread_number; i++)
            {
                end = start + layer_per_thread + (i < remaining_layer ? 1 : 0);
                future_list.push_back(thread_pool.submit_task([&, idx = i, start , end]
                {
                    MakeLayerReturnVector layer_return_vector;
                    for(int i = start; i < end; i++)
                    {
                        layer_return_vector.emplace_back(MakeRandomLayer(i));
                    }
                    std::pair<int, MakeLayerReturnVector> res(idx, std::move(layer_return_vector));
                    return res;
                }));
                start = end;
            }
            std::map<int, MakeLayerReturnVector> layer_map;
            for(auto& fu: future_list)
            {
                layer_map.insert(fu.get());
            }
            for(auto& [key, value] : layer_map)
            {
                for(auto& layer: value)
                {
                    shape_layer_list_.emplace_back(std::move(layer.shape_layer));
                    shape_list_.insert(shape_list_.end(), std::make_move_iterator(layer.shape_list.begin()), std::make_move_iterator(layer.shape_list.end()));
                }
            }
        }
        else 
        {
            for(int i = 0; i < Layer; i++)
            {
                auto layer = MakeRandomLayer(i);
                shape_layer_list_.emplace_back(std::move(layer.shape_layer));
                shape_list_.insert(shape_list_.end(), std::make_move_iterator(layer.shape_list.begin()), std::make_move_iterator(layer.shape_list.end()));
            }
        }
        std::cout << "Create time is: " << timer.SinceStartSecs() << std::endl;
    }
    
    bool Test2D::DrawShapeCallback(Shape* shape)
    {
        auto id = shape->uuid();
        auto render_item = shape_render_item_map_.at(id);
        if(render_item.mode == kRectangle)
        {
            static GLuint rect_indices[6] = { 0, 1, 2, 0, 2, 3};
            std::vector<GLuint> indices;
            indices.resize(6);
            for(int i = 0; i < 6; i++) indices[i] = render_item.offset + rect_indices[i];
            memcpy(&shape_index_buffer_map_[current_shape_index_size], indices.data(), 6 * sizeof(GLuint));
            current_shape_index_size += 6;
        }
        else if(render_item.mode == kCircle)
        {
            std::vector<GLuint> indices;
            indices.resize(3);
            for(int i = 0; i < 3; i++) indices[i] = render_item.offset + i;
            memcpy(&shape_index_buffer_map_[current_shape_index_size], indices.data(), 3 * sizeof(GLuint));
            current_shape_index_size += 3;
        }
        current_viewport_shape++;
        return true;
    }
    
    void Test2D::SetShapeSelect(Shape* shape, bool select)
    {
        auto id = shape->uuid();
        auto render_item = shape_render_item_map_.at(id);
        if(render_item.mode == kRectangle)
        {
            int offset = render_item.offset;
            for(int i = 0; i < 4; i++)
                shape_vertex_buffer_map_[offset++].selected = select;
        }
        else if(render_item.mode == kCircle)
        {
            int offset = render_item.offset;
            for(int i = 0; i < 3; i++)
                shape_vertex_buffer_map_[offset++].selected = select;
        }
    }
    
    void Test2D::ClearSelectShape()
    {
        for(auto shape : select_shape_list_)
        {
            SetShapeSelect(shape, false);
        }
    }
    
    bool Test2D::SelectShapeCallback(Shape* shape)
    {
        SetShapeSelect(shape, true);
        select_shape_list_.push_back(shape);
        return true;
    }
}