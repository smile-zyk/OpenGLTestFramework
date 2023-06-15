#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Orbit Camera
class Camera
{
public:
    explicit Camera(float _fov, float _zNear, float _zFar, float _aspect);
    void rotate(const glm::vec2& motion);
    void move(const glm::vec2& motion);
    void approach(float ratio);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void setCenter(const glm::vec3& pos);
private:
    float fov;
    float zNear;
    float zFar;
    float aspect;
    glm::vec3 position;
    glm::vec3 center;
};

