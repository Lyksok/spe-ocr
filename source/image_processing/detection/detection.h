#ifndef DETECTION_H
#define DETECTION_H

#include <SDL2/SDL.h>
#include <err.h>

#include "structures.h"
#include "frequencer.h"
#include "list.h"
#include "point.h"

// Useful bounding box and point operations
void sort_bounding_boxes(BoundingBox*** boxes, int len, int (*asc)(BoundingBox*, BoundingBox*));

double get_distance_points(Point p1, Point p2);
int is_already_closest_of(Point src_box, Point dest_box, Point* src, Point* dest, int l);
BoundingBox* closest_bounding_boxes(BoundingBox* box, BoundingBox** boxes, int len);
void get_all_links(BoundingBox** boxes, int len, Point** src, Point** dest);

// Main function for detection
BoundingBox** get_char_boxes(struct list* char_boxes, int* res_len);

BoundingBox* get_grid_box(SDL_Surface* surface);
BoundingBox* get_word_list_box(SDL_Surface* surface);
BoundingBox** get_word_boxes(SDL_Surface* surface, int* word_count);

#endif /* DETECTION_H */