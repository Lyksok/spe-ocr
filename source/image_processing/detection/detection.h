#ifndef DETECTION_H
#define DETECTION_H

#include <SDL2/SDL.h>
#include "list.h"
#include "filtering.h"
#include "frequencer.h"
#include "point.h"

struct list* detect_characters(SDL_Surface* surface, struct list* box_list);
BoundingBox** detect_grid(SDL_Surface* surface, struct list* char_list,
        int* width, int* height);

#endif /* DETECTION_H */
