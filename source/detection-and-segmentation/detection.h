#ifndef DETECTION_H
#define DETECTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../image_processing/detection/bounding-box.h"
#include "../image_processing/detection/filtering.h"
#include "../image_processing/detection/frequencer.h"
#include "../image_processing/detection/list.h"
#include "../image_processing/detection/point.h"
#include "../image_processing/detection/structures.h"
#include "../image_processing/utils/pixel_utils.h"
#include "cluster-detection.h"

#include "detect_histograms.h"

// Visual debugging
void draw_rect(SDL_Renderer *renderer, int rect[4]);
void draw_rects(SDL_Renderer *renderer, int rect[][4], size_t len);
void draw_line(SDL_Renderer *renderer, Point p1, Point p2);
void draw_lines(SDL_Renderer *renderer, Point *src, Point *dest, int len);

// Useful bounding box and point operations
Point get_bounding_box_center(BoundingBox *b);
Point get_points_average(Point p1, Point p2);
void sort_bounding_boxes(BoundingBox ***boxes, int len,
                         int (*asc)(BoundingBox *, BoundingBox *));

double get_distance_points(Point p1, Point p2);
int is_already_closest_of(Point src_box, Point dest_box, Point *src,
                          Point *dest, int l);
BoundingBox *closest_bounding_boxes(BoundingBox *box, BoundingBox **boxes,
                                    int len);
void get_all_links(BoundingBox **boxes, int len, Point **src, Point **dest);

// Main function for detection
BoundingBox **get_char_boxes(SDL_Surface *surface, int *res_len,
                             struct parameters *param);

BoundingBox *get_grid_box(SDL_Surface *surface, struct parameters *param);
BoundingBox *get_word_list_box(SDL_Surface *surface, struct parameters *param);
BoundingBox **get_word_boxes(SDL_Surface *surface, int *word_count,
                             struct parameters *param);

#endif /* DETECTION_H */