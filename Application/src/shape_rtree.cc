#include "shape_rtree.h"

void ShapeRTree::Insert(Shape* shape)
{
    BoundingBox box = shape->GetBoundingBox();
    glm::vec2 min = box.GetMin();
    glm::vec2 max = box.GetMax();
    rtree_.Insert(&min[0], &max[0], shape);
}

int ShapeRTree::Search(BoundingBox box, ShapeRTreeCallback callback)
{
    glm::vec2 min = box.GetMin();
    glm::vec2 max = box.GetMax();
    return rtree_.Search(&min[0], &max[0], callback);
}
