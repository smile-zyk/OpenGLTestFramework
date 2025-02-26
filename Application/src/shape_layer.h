#pragma once

#include "shape_rtree.h"

class ShapeLayer
{
public:
    ShapeLayer(int layer_id);
    ShapeRTree& GetRTree();
    int id();
    float depth();
    void set_depth(float depth);
private:
    ShapeRTree shape_rtree_;
    float depth_{};
    int id_{};
};