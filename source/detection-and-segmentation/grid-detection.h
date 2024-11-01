#ifndef GRID_DETECTION_H
#define GRID_DETECTION_H

#include <SDL2/SDL.h>
#include "structures.h"

void draw_rect(SDL_Renderer *renderer, int rect[4]);
void draw_rects(SDL_Renderer* renderer, int rect[][4], size_t len);
void draw_line(SDL_Renderer *renderer, Point p1, Point p2);
void draw_lines(SDL_Renderer* renderer, Point* src, Point* dest, int len);
Point get_bounding_box_center(BoundingBox b);
Point get_points_average(Point p1, Point p2);
int ascending_bounding_box(BoundingBox* b1, BoundingBox* b2);
void sort_bounding_boxes(BoundingBox*** boxes, int len, int (*asc)(BoundingBox*, BoundingBox*));
double get_distance_points(Point p1, Point p2);
int is_already_closest_of(Point src_box, Point dest_box, Point* src, Point* dest, int l);
BoundingBox* closest_bounding_boxes(BoundingBox* box, BoundingBox** boxes, int len, Point* src, Point* dest, int l);
void get_all_links(BoundingBox** boxes, int len, Point** src, Point** dest);

#endif /* GRID_DETECTION_H */