#pragma once

#include "shape.h"
#include "boundingbox.h"

#include <RTree.h>

#include <functional>
#include <vector>

class ShapeRTree
{
public:
    typedef std::function<bool(Shape*)> ShapeRTreeCallback;
    void Insert(Shape* shape);
    int Search(BoundingBox box, ShapeRTreeCallback callback);
    std::vector<Shape*> GetShapes();
private:
    typedef RTree<Shape*, float, 2> RTree2D;
    RTree2D rtree_;    
};