#ifndef FREQUENCER_H
#define FREQUENCER_H

#include "../utils/helper.h"
#include "bounding-box.h"
#include "list.h"
#include "structures.h"
#include <SDL2/SDL.h>

// Get nb of box per column and row
void freq_box_nb(SDL_Surface *surface, struct list *box_list,
                 int **nb_box_per_col, int **nb_box_per_row);
int freq_most_frequent(int *freq_list, int size);

// Get the median value from array of int
int freq_get_median_from_array(int len, int *values);

// Compute the closer distance from another box
int freq_compute_minimal_distance(struct list *box, struct list *box_list);

// Compute all the closer distances from each box to others
int *freq_compute_all_minimal_distances(struct list *box_list);
int dist2(Point p1, Point p2);

#endif /* FREQUENCER_H */
