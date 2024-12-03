#ifndef CLUSTER_DETECTION_H
#define CLUSTER_DETECTION_H

#include <SDL2/SDL.h>
#include <err.h>
#include "bounding-box.h"
#include "list.h"
#include "../utils/pixel_utils.h"
#include "../utils/helper.h"

// Get all bounding boxes of the grid
void compute_bounding_boxes(SDL_Surface *surface, struct list* box_list);

#endif /* CLUSTER_DETECTION_H */
