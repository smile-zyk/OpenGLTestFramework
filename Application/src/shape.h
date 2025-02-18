#pragma once

#include "boundingbox.h"
#include <glm/glm.hpp>
#include <string>
#include <uuid.h>

enum ShapeType
{
    kRectangle,
    kCircle,
};

class ShapeRenderer;

class Shape
{
public:
    virtual ~Shape() = default;
    virtual BoundingBox GetBoundingBox() const = 0;
    virtual std::string ToString() const = 0;
    virtual ShapeType GetShapeType() const = 0; 
    virtual void Accept(ShapeRenderer* renderer) = 0;
    uuids::uuid uuid();
    glm::vec4 color() { return color_; }
    float layer() { return layer_; }    
    void set_color(const glm::vec4& color) { color_ = color; }
protected:
    Shape(float layer);
    Shape() = delete;
private:
    uuids::uuid uuid_;
    glm::vec4 color_;
    float layer_;
};

class Rectangle : public Shape
{
public:
    Rectangle(float x, float y, float width, float height, float layer);
    Rectangle(const glm::vec2& origin, const glm::vec2& size, float layer);
    BoundingBox GetBoundingBox() const override;
    std::string ToString() const override;
    ShapeType GetShapeType() const override;
    void Accept(ShapeRenderer* renderer) override;
    glm::vec2 origin() { return origin_; }
    glm::vec2 size() { return size_; }
private:
    glm::vec2 origin_{0.f};
    glm::vec2 size_{0.f};
};

class Circle : public Shape
{
public:
    Circle(float center_x, float center_y, float radius, float layer);
    Circle(const glm::vec2& center, float radius, float layer);
    ShapeType GetShapeType() const override;
    BoundingBox GetBoundingBox() const override;
    std::string ToString() const override;
    void Accept(ShapeRenderer* renderer) override;
    glm::vec2 center() { return center_; }
    float radius() { return radius_; }
private:
    glm::vec2 center_{0.f};
    float radius_{};
};

class ShapeRenderer
{
public:
    virtual void Draw(Circle* circle) = 0;
    virtual void Draw(Rectangle* rect) = 0;
};