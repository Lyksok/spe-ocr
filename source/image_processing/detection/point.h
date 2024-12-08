#ifndef POINT_H
#define POINT_H

#include "structures.h"

// Check if two points are equal
int point_are_equal(Point p1, Point p2);

// Check if a point is in a box
int point_is_in_box(Point point, BoundingBox *box);

// Check if a point x componant is within the box
int point_is_in_box_width(Point point, BoundingBox *box);

// Check if a point y componant is within the box
int point_is_in_box_height(Point point, BoundingBox *box);

// Check if a point is in the list of box
int point_is_in_box_list(Point point, struct list *list);

#endif /* POINT_H */
