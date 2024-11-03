#ifndef DETECTION_H
#define DETECTION_H

#include <SDL2/SDL.h>
#include "structures.h"

// Visual debugging
void draw_rect(SDL_Renderer *renderer, int rect[4]);
void draw_rects(SDL_Renderer* renderer, int rect[][4], size_t len);
void draw_line(SDL_Renderer *renderer, Point p1, Point p2);
void draw_lines(SDL_Renderer* renderer, Point* src, Point* dest, int len);

// Useful bounding box and point operations
Point get_bounding_box_center(BoundingBox* b);
Point get_points_average(Point p1, Point p2);
void sort_bounding_boxes(BoundingBox*** boxes, int len, int (*asc)(BoundingBox*, BoundingBox*));

// Main function for detection
BoundingBox** get_char_boxes(SDL_Surface* surface, int* res_len);
BoundingBox* get_grid_box(SDL_Surface* surface);
BoundingBox* get_word_list_box(SDL_Surface* surface);
BoundingBox** get_word_boxes(SDL_Surface* surface, int* word_count);

#endif /* DETECTION_H */
