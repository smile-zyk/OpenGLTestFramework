#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

struct Rect
{
    glm::vec2 p1{0.f};
    glm::vec2 p2{0.f};
    glm::vec2 GetMin() const;
    glm::vec2 GetMax() const;
    void Reset();
    float Area() const;
    bool IsValid() const;
};