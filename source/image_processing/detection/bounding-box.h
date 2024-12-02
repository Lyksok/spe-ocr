#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "structures.h"

int box_are_equal(BoundingBox* b1, BoundingBox* b2);
void box_expand_coord(BoundingBox* box, int x, int y);
void box_init_coord(BoundingBox* box, int x, int y);
void box_init_coord_from_box(BoundingBox* src, BoundingBox* dst);
int box_get_height(BoundingBox* box);
int box_get_width(BoundingBox* box);
Point box_get_center(BoundingBox* box);

/*
// Structure comparisons
int are_equal_point(Point p1, Point p2);

// Heap representation operations
int add_box(BoundingBox box, BoundingBox*** boxes, int len);
int remove_box(BoundingBox box, BoundingBox*** boxes, int len);

// General array representation operations
void free_boxes(BoundingBox** boxes, int len);
int contains_box(BoundingBox box, BoundingBox** boxes, int len);
void print_boxes(BoundingBox** boxes, int len);
int is_minimal(BoundingBox** boxes, int len);
int get_nb_of_boxes(BoundingBox** boxes, int len);
BoundingBox** get_minimal(BoundingBox** boxes, int len, int* res_len);
int average_box_height(BoundingBox** boxes, int len);
int average_box_width(BoundingBox** boxes, int len);
int is_in_box(BoundingBox* box, Point point);
int is_in_boxes(BoundingBox** boxes, int len, Point point);
Point* bounding_box_to_points(BoundingBox** boxes, int len);
*/

#endif /* BOUNDING_BOX_H */
