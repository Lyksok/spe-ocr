#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "structures.h"
#include <SDL2/SDL.h>

// Structure comparisons
int are_equal_point(Point p1, Point p2);
int are_equal_box(BoundingBox b1, BoundingBox b2);

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

#endif /* BOUNDING_BOX_H */
