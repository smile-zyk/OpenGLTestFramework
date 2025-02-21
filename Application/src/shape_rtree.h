#pragma once

#include "shape.h"
#include "boundingbox.h"

#include <RTree.h>

#include <functional>

class ShapeRTree
{
public:
    typedef std::function<bool(Shape*)> ShapeRTreeCallback;
    void Insert(Shape* shape);
    int Search(BoundingBox box, ShapeRTreeCallback callback);
private:
    typedef RTree<Shape*, float, 2> RTree2D;
    RTree2D rtree_;    
};

