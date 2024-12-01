#ifndef FILTERING_H
#define FILTERING_H

#include "bounding-box.h"
#include "list.h"

void filter_wrong_size_boxes(struct list* box_list);
void filter_wrong_size_boxes_threshold(struct list* box_list);

#endif /* FILTERING_H */
