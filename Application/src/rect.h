#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>

struct RectVertex
{
    glm::vec2 screen_uv;
    glm::vec2 tex_coord;
};

struct RectRenderData
{
    std::vector<RectVertex> rect_vertices;
    std::vector<GLuint> rect_indices;
};

struct Rect
{
    glm::vec2 p1{0.f};
    glm::vec2 p2{0.f};
    glm::vec2 GetMin() const;
    glm::vec2 GetMax() const;
    RectRenderData GetRenderData() const;
    void Reset();
    float Area() const;
    bool IsValid() const;
};