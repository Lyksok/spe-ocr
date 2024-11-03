#ifndef CLUSTER_DETECTION_H
#define CLUSTER_DETECTION_H

#include "structures.h"
#include <SDL2/SDL.h>

void get_bounding_boxes(SDL_Surface *surface, BoundingBox ***boxes);

#endif /* CLUSTER_DETECTION_H */