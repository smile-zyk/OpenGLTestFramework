#include "rect.h"
#include <glm/common.hpp>

glm::vec2 Rect::GetMin() const
{
    return glm::vec2{std::fmin(p1.x, p2.x), std::fmin(p1.y, p2.y)};
}

glm::vec2 Rect::GetMax() const
{
    return glm::vec2{std::fmax(p1.x, p2.x), std::fmax(p1.y, p2.y)};
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
