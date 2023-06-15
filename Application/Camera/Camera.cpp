#include "Camera.h"

Camera::Camera(float _fov, float _zNear, float _zFar, float _aspect)
    :fov(_fov), zNear(_zNear), zFar(_zFar), aspect(_aspect)
{
    center = { 0.f,0.f,0.f };
    position = center;
    position.z += 4.f;
}

void Camera::rotate(const glm::vec2& motion)
{
    glm::vec3 centerVec = position - center;
    float radius = glm::length(centerVec);
    float yaw = std::atan2(centerVec.x, centerVec.z);
    float pitch = std::asin(centerVec.y / radius);
    float factor = M_PI * 2.f;
    glm::vec3 offset;
    yaw -= motion.x * factor;
    pitch += motion.y * factor;

    if (pitch + M_PI_2 < DBL_EPSILON) pitch = -glm::radians(89.9f);
    if (pitch - M_PI_2 > DBL_EPSILON) pitch = glm::radians(89.9f);

    offset.x = (radius * (float)std::cos(pitch) * (float)std::sin(yaw));
    offset.y = (radius * (float)std::sin(pitch));
    offset.z = (radius * (float)std::cos(pitch) * (float)std::cos(yaw));

    position = center + offset;
}

void Camera::move(const glm::vec2& motion)
{
    glm::vec3 positionVec = center - position;
    glm::vec3 forward = glm::normalize(positionVec);
    glm::vec3 left = glm::normalize(glm::cross({ 0.0f,1.0f,0.0f }, forward));
    glm::vec3 up = glm::normalize(glm::cross(forward, left));

    float distance = glm::length(positionVec);
    float factor = distance * (float)tan(glm::radians(fov) / 2) * 2;
    glm::vec3 deltaX = factor * aspect * motion.x * left;
    glm::vec3 deltaY = factor * motion.y * up;
    center += (deltaX + deltaY);
    position += (deltaX + deltaY);
}

void Camera::approach(float ratio)
{
    glm::vec3 centerVec = position - center;
    float radius = glm::length(centerVec);
    float yaw = (float)std::atan2(centerVec.x, centerVec.z);
    float pitch = (float)std::asin(centerVec.y / radius);
    glm::vec3 offset;
    radius *= (float)std::pow(0.95, ratio);
    offset.x = (radius * (float)std::cos(pitch) * (float)std::sin(yaw));
    offset.y = (radius * (float)std::sin(pitch));
    offset.z = (radius * (float)std::cos(pitch) * (float)std::cos(yaw));
    position = center + offset;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, center, { 0.0f,1.0f,0.0f });
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}

void Camera::setCenter(const glm::vec3& pos)
{
    glm::vec3 offset = position - center;
    center = pos;
    position = center + offset;
}
