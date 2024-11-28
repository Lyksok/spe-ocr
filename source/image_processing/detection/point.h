#ifndef POINT_H
#define POINT_H

#include "bounding-box.h"

typedef struct Point
{
    int x;
    int y;
} Point;

// Check if two points are equal
int point_are_equal(Point p1, Point p2);

// Check if a point is in a box
int point_is_in_box(Point p, BoundingBox* box);

// Check if a point is in the list of box
int point_is_in_box_list(Point p, struct list* list);

#endif /* POINT_H */
