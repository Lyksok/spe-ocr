#ifndef DETECTION_H
#define DETECTION_H

#include <SDL2/SDL.h>
#include "list.h"
#include "filtering.h"

struct list* detect_characters(SDL_Surface* surface, struct list* box_list);

#endif /* DETECTION_H */
