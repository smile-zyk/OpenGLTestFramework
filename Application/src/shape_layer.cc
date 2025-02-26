#include "shape_layer.h"


ShapeLayer::ShapeLayer(int layer_id) : id_(layer_id)
{
    depth_ = static_cast<float>(id_);
}

ShapeRTree& ShapeLayer::GetRTree()
{
    return shape_rtree_;
}

int ShapeLayer::id()
{
    return id_;
}

float ShapeLayer::depth()
{
    return depth_;
}

void ShapeLayer::set_depth(float depth)
{
    depth_ = depth;
}