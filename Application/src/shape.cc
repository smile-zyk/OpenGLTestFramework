#include "shape.h"
#include <sstream>

Rectangle::Rectangle(float x, float y, float width, float height)
{
    origin.x = x;
    origin.y = y;
    size.x = width;
    size.y = height;       
}

Rectangle::Rectangle(const glm::vec2& origin_, const glm::vec2& size_)
{
    origin = origin_;
    size = size_;
}

BoundingBox Rectangle::GetBoundingBox() const 
{
    return {origin, origin + size};
}

std::string Rectangle::ToString() const 
{
    std::stringstream ss;
    ss << "Rect: origin(" << origin.x << ',' << origin.y << "), size(" << size.x << ',' << size.y << ')';
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

Circle::Circle(float center_x, float center_y, float radius_)
{
    center.x = center_x;
    center.y = center_y;
    radius = radius_;
}

Circle::Circle(const glm::vec2& center_, float radius_)
{
    center = center_;
    radius = radius_;
}

ShapeType Circle::GetShapeType() const 
{
    return ShapeType::kCircle;
}

BoundingBox Circle::GetBoundingBox() const 
{
    return { center - radius / 2.f, center + radius / 2.f };
}

std::string Circle::ToString() const 
{
    std::stringstream ss;
    ss << "Circle: center(" << center.x << ',' << center.y << ") radius: " << radius << ')';
    return ss.str();
}

void Circle::Accept(ShapeRenderer* renderer)
{
    renderer->Draw(this);
}

Shape::Shape()
{
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};
    
    uuid_ = gen();
}
