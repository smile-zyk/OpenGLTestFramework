#pragma once
#include "camera.h"

#include <glm/glm.hpp>

//Orbit Camera
class Camera3D : public Camera
{
public:
    explicit Camera3D(float _fov, float _z_near_, float _zFar, float _aspect);
    void rotate(const glm::vec2& motion);
    void move(const glm::vec2& motion);
    void approach(float ratio);
    void set_center(const glm::vec3& pos);
    void UpdateViewMatrix() override;
    void UpdateProjectionMatrix() override;
private:
    float fov_;
    float z_near_;
    float z_far_;
    float aspect_;
    glm::vec3 position_;
    glm::vec3 center_;
};

