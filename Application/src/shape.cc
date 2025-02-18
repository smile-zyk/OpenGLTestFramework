#include "shape.h"
#include <sstream>

Rectangle::Rectangle(float x, float y, float width, float height, float layer): Shape(layer)
{
    origin_.x = x;
    origin_.y = y;
    size_.x = width;
    size_.y = height;       
}

Rectangle::Rectangle(const glm::vec2& origin, const glm::vec2& size, float layer): Shape(layer)
{
    origin_ = origin;
    size_ = size;
}

BoundingBox Rectangle::GetBoundingBox() const 
{
    return {origin_, origin_ + size_};
}

std::string Rectangle::ToString() const 
{
    std::stringstream ss;
    ss << "Rect: origin(" << origin_.x << ',' << origin_.y << "), size(" << size_.x << ',' << size_.y << ')';
    return ss.str();
}

ShapeType Rectangle::GetShapeType() const 
{
    return ShapeType::kRectangle;
}

void Rectangle::Accept(ShapeRenderer* renderer)
{
    renderer->Draw(this);
}

Circle::Circle(float center_x, float center_y, float radius, float layer) : Shape(layer)
{
    center_.x = center_x;
    center_.y = center_y;
    radius_ = radius;
}

Circle::Circle(const glm::vec2& center, float radius, float layer) : Shape(layer)
{
    center_ = center;
    radius_ = radius;
}

ShapeType Circle::GetShapeType() const 
{
    return ShapeType::kCircle;
}

BoundingBox Circle::GetBoundingBox() const 
{
    return { center_ - radius_ / 2.f, center_ + radius_ / 2.f };
}

std::string Circle::ToString() const 
{
    std::stringstream ss;
    ss << "Circle: center(" << center_.x << ',' << center_.y << ") radius: " << radius_ << ')';
    return ss.str();
}

void Circle::Accept(ShapeRenderer* renderer)
{
    renderer->Draw(this);
}

Shape::Shape(float layer) : layer_(layer)
{
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};
    
    uuid_ = gen();
}
