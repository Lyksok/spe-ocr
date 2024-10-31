#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "structures.h"

int add_box(BoundingBox box, BoundingBox*** boxes, int len);
int are_equal_point(Point p1, Point p2);
int are_equal_box(BoundingBox b1, BoundingBox b2);
int remove_box(BoundingBox box, BoundingBox*** boxes, int len);

#endif /* BOUNDING_BOX_H */