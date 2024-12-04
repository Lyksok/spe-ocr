#ifndef FILTERING_H
#define FILTERING_H

#include <SDL2/SDL.h>
#include "bounding-box.h"
#include "list.h"
#include "../utils/converting.h"

void filter_wrong_size_boxes(SDL_Surface* surface, struct list* box_list);
void filter_wrong_size_boxes_threshold(SDL_Surface* surface, struct list* box_list);

// Filter single boxes that are probably not part of the grid
struct list* filter_probable_grid_boxes(struct list* box_list, int wanted_nb);

#endif /* FILTERING_H */
