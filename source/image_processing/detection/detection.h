#ifndef DETECTION_H
#define DETECTION_H

#include <SDL2/SDL.h>
#include "list.h"
#include "filtering.h"
#include "frequencer.h"
#include "point.h"

struct list* detect_characters(SDL_Surface* surface, struct list* box_list, struct parameters* param);
BoundingBox** detect_grid(SDL_Surface* surface, struct list* char_list,
        int* width, int* height, struct parameters* param);

#endif /* DETECTION_H */
