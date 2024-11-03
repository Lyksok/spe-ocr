#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <SDL2/SDL.h>

#include "structures.h"

void save_bounding_box(SDL_Surface* surface, BoundingBox* box);
void save_bounding_boxes(SDL_Surface* surface, BoundingBox** boxes, int len);

#endif /* SEGMENTATION_H */
