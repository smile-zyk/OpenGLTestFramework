#pragma once

#include "camera.h"
#include <glm/glm.hpp>

class Camera2D : public Camera
{
public:
    Camera2D(int width, int height, float near, float far);
    void Move(glm::vec2 motion);
    void Zoom(float zoom);
    void SetViewPort(int width, int height);
    void SetNearFar(float near, float far);
    void UpdateViewMatrix() override;
    void UpdateProjectionMatrix() override;
private:
    glm::vec2 center_{};
    float zoom_{1.f};
    int width_;
    int height_;
    float near_;
    float far_;
};