#ifndef DETECTION_H
#define DETECTION_H

#include <SDL2/SDL.h>
#include "list.h"
#include "filtering.h"
#include "frequencer.h"
#include "point.h"

struct list* detect_characters(SDL_Surface* surface, struct list* box_list, struct parameters* param);
int dist2(Point p1, Point p2);
BoundingBox** detect_grid(SDL_Surface* surface, struct list* char_list,
        int* width, int* height, struct parameters* param);

#endif /* DETECTION_H */
