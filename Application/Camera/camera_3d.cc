#include "camera_3d.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>

Camera3D::Camera3D(float fov, float z_near, float z_far, float aspect)
    :fov_(fov), z_near_(z_near), z_far_(z_far), aspect_(aspect)
{
    center_ = { 0.f,0.f,0.f };
    position_ = center_;
    position_.z += 4.f;
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Camera3D::rotate(const glm::vec2& motion)
{
    glm::vec3 center_Vec = position_ - center_;
    float radius = glm::length(center_Vec);
    float yaw = std::atan2(center_Vec.x, center_Vec.z);
    float pitch = std::asin(center_Vec.y / radius);
    float factor = static_cast<float>(M_PI) * 2.f;
    glm::vec3 offset;
    yaw -= motion.x * factor;
    pitch += motion.y * factor;

    if (pitch + M_PI_2 < DBL_EPSILON) pitch = -glm::radians(89.9f);
    if (pitch - M_PI_2 > DBL_EPSILON) pitch = glm::radians(89.9f);

    offset.x = (radius * (float)std::cos(pitch) * (float)std::sin(yaw));
    offset.y = (radius * (float)std::sin(pitch));
    offset.z = (radius * (float)std::cos(pitch) * (float)std::cos(yaw));

    position_ = center_ + offset;
    UpdateViewMatrix();
}

void Camera3D::move(const glm::vec2& motion)
{
    glm::vec3 position_Vec = center_ - position_;
    glm::vec3 forward = glm::normalize(position_Vec);
    glm::vec3 left = glm::normalize(glm::cross({ 0.0f,1.0f,0.0f }, forward));
    glm::vec3 up = glm::normalize(glm::cross(forward, left));

    float distance = glm::length(position_Vec);
    float factor = distance * (float)tan(glm::radians(fov_) / 2) * 2;
    glm::vec3 deltaX = factor * aspect_ * motion.x * left;
    glm::vec3 deltaY = factor * motion.y * up;
    center_ += (deltaX + deltaY);
    position_ += (deltaX + deltaY);
    UpdateViewMatrix();
}

void Camera3D::approach(float ratio)
{
    glm::vec3 center_Vec = position_ - center_;
    float radius = glm::length(center_Vec);
    float yaw = (float)std::atan2(center_Vec.x, center_Vec.z);
    float pitch = (float)std::asin(center_Vec.y / radius);
    glm::vec3 offset;
    radius *= (float)std::pow(0.95, ratio);
    offset.x = (radius * (float)std::cos(pitch) * (float)std::sin(yaw));
    offset.y = (radius * (float)std::sin(pitch));
    offset.z = (radius * (float)std::cos(pitch) * (float)std::cos(yaw));
    position_ = center_ + offset;
    UpdateViewMatrix();
}

void Camera3D::set_center(const glm::vec3& pos)
{
    glm::vec3 offset = position_ - center_;
    center_ = pos;
    position_ = center_ + offset;
    UpdateViewMatrix();
}

void Camera3D::UpdateViewMatrix()
{
    view_matrix_ = glm::lookAt(position_, center_, { 0.0f,1.0f,0.0f });
}

void Camera3D::UpdateProjectionMatrix()
{
    projection_matrix_ = glm::perspective(glm::radians(fov_), aspect_, z_near_, z_far_);
}


