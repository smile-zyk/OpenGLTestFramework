#include "rect.h"
#include <glm/common.hpp>

glm::vec2 Rect::GetMin() const
{
    return glm::clamp(glm::vec2{std::fmin(p1.x, p2.x), std::fmin(p1.y, p2.y)}, 0.f, 1.f);
}

glm::vec2 Rect::GetMax() const
{
    return glm::clamp(glm::vec2{std::fmax(p1.x, p2.x), std::fmax(p1.y, p2.y)}, 0.f, 1.f);
}

RectRenderData Rect::GetRenderData() const
{
    glm::vec2 p1 = GetMin();
    glm::vec2 p3 = GetMax();
    glm::vec2 p2 = {p3.x, p1.y};
    glm::vec2 p4 = {p1.x, p3.y};

    glm::vec2 tex1 = {0.f, 0.f};
    glm::vec2 tex2 = {1.f, 0.f};
    glm::vec2 tex3 = {1.f, 1.f};
    glm::vec2 tex4 = {0.f, 1.f};

    RectVertex v1 = {p1, tex1};
    RectVertex v2 = {p2, tex2};
    RectVertex v3 = {p3, tex3};
    RectVertex v4 = {p4, tex4};

    RectRenderData res;
    res.rect_vertices.push_back(v1);
    res.rect_vertices.push_back(v2);
    res.rect_vertices.push_back(v3);
    res.rect_vertices.push_back(v4);

    res.rect_indices = {0, 2, 3, 0, 1, 2};

    return res;
}

void Rect::Reset()
{
    p1 = {0.f, 0.f};
    p2 = {0.f, 0.f};
}

float Rect::Area() const
{
    glm::vec2 size = GetMax() - GetMin();
    return size.x * size.y;
}

bool Rect::IsValid() const
{
    return Area() > 1e-6;
}
