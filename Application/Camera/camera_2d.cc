#include "camera_2d.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>

Camera2D::Camera2D(int width, int height, float near, float far): width_(width), height_(height), near_(near), far_(far)
{
    UpdateProjectionMatrix();
    UpdateViewMatrix();
}

void Camera2D::Move(glm::vec2 motion)
{
    float factor = width_ / zoom_;
    center_ += motion * factor;
    UpdateViewMatrix();
}

void Camera2D::Zoom(float zoom)
{
    zoom_ = zoom;
    UpdateProjectionMatrix();
}

void Camera2D::SetViewPort(int width, int height)
{
    width_ = width;
    height_ = height;
    UpdateProjectionMatrix();
}

void Camera2D::SetNearFar(float near, float far)
{
    near_ = near;
    far_ = far;
    UpdateProjectionMatrix();
}

void Camera2D::UpdateViewMatrix()
{
    view_matrix_ = glm::translate(glm::mat4(1.0), glm::vec3(center_.x, center_.y, 0));
}

void Camera2D::UpdateProjectionMatrix()
{
    projection_matrix_ = glm::ortho(-width_ / (2.f * zoom_) , width_ / (2.f * zoom_) , -height_ / (2.f * zoom_), height_ / (2.f * zoom_), near_, far_);
}
