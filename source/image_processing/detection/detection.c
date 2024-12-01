#include "detection.h"

// Compute all bounding_boxes

/* Detects characters in the grid by filtering boxes
 * using their height and their width to find non-characters
 * Returns a new list with characters' boxes
 */
struct list* detect_characters(struct list* box_list)
{
    filter_wrong_size_boxes_threshold(box_list);
}

/* Find dimensions of the grid
 * If found return 0 and set width and height
 * Else return 1
 */
int detect_grid_dimensions(int* width, int* height)
{
    // TODO
}
