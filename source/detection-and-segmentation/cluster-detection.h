#ifndef CLUSTER_DETECTION_H
#define CLUSTER_DETECTION_H

#include <SDL2/SDL.h>
#include <err.h>
#include "../image_processing/detection/structures.h"
#include "../image_processing/detection/list.h"
#include "../image_processing/detection/bounding-box.h"
#include "../image_processing/utils/pixel_utils.h"

void compute_bounding_boxes(SDL_Surface *surface, struct list* box_list);

#endif /* CLUSTER_DETECTION_H */