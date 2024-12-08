#ifndef FILTERING_H
#define FILTERING_H

#include <SDL2/SDL.h>
#include "bounding-box.h"
#include "list.h"
#include "../utils/converting.h"
#include "../parameters.h"

void filter_wrong_size_boxes_threshold(SDL_Surface* surface,
	struct list* box_list, struct parameters* param);

#endif /* FILTERING_H */
