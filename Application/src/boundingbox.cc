#include "boundingbox.h"

glm::vec2 BoundingBox::GetMin() const
{
    return glm::vec2{std::fmin(p1.x, p2.x), std::fmin(p1.y, p2.y)};
}

glm::vec2 BoundingBox::GetMax() const
{
    return glm::vec2{std::fmax(p1.x, p2.x), std::fmax(p1.y, p2.y)};
}

void BoundingBox::Reset()
{
    p1 = {0.f, 0.f};
    p2 = {0.f, 0.f};
}

float BoundingBox::Area() const
{
    glm::vec2 size = GetMax() - GetMin();
    return size.x * size.y;
}

bool BoundingBox::IsValid() const
{
    return Area() > 1e-6;
}
