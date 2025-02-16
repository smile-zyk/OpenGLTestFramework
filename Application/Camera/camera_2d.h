#pragma once

#include "camera.h"
#include "boundingbox.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Camera2D : public Camera
{
public:
    Camera2D(int width, int height, float near, float far);
    void Move(glm::vec2 motion);
    void Zoom(float zoom);
    void SetWindowSize(int width, int height);
    void SetNearFar(float near, float far);
    void UpdateViewMatrix() override;
    void UpdateProjectionMatrix() override;
    void set_center(glm::vec2 center);
    BoundingBox GetViewport();
    glm::vec2 center();
    float near();
    float far();
private:
    glm::vec2 center_{};
    float zoom_{1.f};
    int width_;
    int height_;
    float near_;
    float far_;
};