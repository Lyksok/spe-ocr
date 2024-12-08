#ifndef DETECT_HISTOGRAMS_H
#define DETECT_HISTOGRAMS_H

#include "../image_processing/detection/structures.h"
#include <SDL2/SDL.h>

void get_col_histogram(SDL_Surface *surface, int **histogram);
void get_row_histogram(SDL_Surface *surface, int **histogram);
BoundingBox ***group_by_rows(SDL_Surface *surface, BoundingBox **boxes, int len,
                             int *n_len);
BoundingBox ***group_by_cols(SDL_Surface *surface, BoundingBox **boxes, int len,
                             int *n_len);
int *get_row_box_histogram(SDL_Surface *surface, BoundingBox **boxes, int len);
int *get_col_box_histogram(SDL_Surface *surface, BoundingBox **boxes, int len);
void print_int_histogram(int *histogram, int len);
int *get_row_bounded_box_histogram(BoundingBox **boxes, BoundingBox *box,
                                   int len);
int is_in_box(BoundingBox *box, Point point);
int is_in_boxes(BoundingBox **boxes, int len, Point point);

int get_most_frequent(int *histogram, int len);

#endif /* DETECT_HISTOGRAMS_H */
