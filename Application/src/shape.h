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

struct Shape
{
public:
    virtual ~Shape() = default;
    virtual BoundingBox GetBoundingBox() const = 0;
    virtual std::string ToString() const = 0;
    virtual ShapeType GetShapeType() const = 0; 
    virtual void Accept(ShapeRenderer* renderer) = 0;
    uuids::uuid uuid();
protected:
    Shape();
private:
    uuids::uuid uuid_;
};

struct Rectangle : public Shape
{
    Rectangle(float x, float y, float width, float height);
    Rectangle(const glm::vec2& origin, const glm::vec2& size);
    BoundingBox GetBoundingBox() const override;
    std::string ToString() const override;
    ShapeType GetShapeType() const override;
    void Accept(ShapeRenderer* renderer) override;
    glm::vec2 origin{0};
    glm::vec2 size{0};
};

struct Circle : public Shape
{
public:
    Circle(float center_x, float center_y, float radius);
    Circle(const glm::vec2& center, float radius);
    ShapeType GetShapeType() const override;
    BoundingBox GetBoundingBox() const override;
    std::string ToString() const override;
    void Accept(ShapeRenderer* renderer) override;
    glm::vec2 center;
    float radius;
};

struct ShapeRenderer
{
public:
    virtual void Draw(Circle* circle) = 0;
    virtual void Draw(Rectangle* rect) = 0;
};