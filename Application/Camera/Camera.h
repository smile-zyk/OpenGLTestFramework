#pragma once

#include <glm/mat4x4.hpp>

class Camera
{
public:
    glm::mat4 view_matrix() { return view_matrix_;}
    glm::mat4 projection_matrix() { return projection_matrix_; }
    glm::mat4 GetViewProjectionMatrix() { return view_matrix_ * projection_matrix_; }
    virtual void UpdateViewMatrix() = 0;
    virtual void UpdateProjectionMatrix() = 0;
protected:
    glm::mat4 projection_matrix_; 
    glm::mat4 view_matrix_; 
};