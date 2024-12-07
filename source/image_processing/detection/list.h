#ifndef LIST_H
#define LIST_H

#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdlib.h>
#include "structures.h"
#include "bounding-box.h"
#include "../utils/helper.h"

// Initialize the sentinel of the list
void init_list(struct list* list);

// Initialize a new list element by using calloc
// and assigning an id
struct list* list_new_list();

// Checks if the list is empty
int list_is_empty(struct list* list);

// Returns the length of list
size_t list_len(struct list *list);

// Check if list contains a box
int list_contains(struct list* list, BoundingBox* box);

// Insert a bounding box in front of the list
void list_push_front(struct list* list, BoundingBox* box);

// Insert a bounding box in front of the list
void list_push_tail(struct list* list, BoundingBox* box);

// Insert a bounding box in the list (keeping the list sorted)
void list_insert(struct list* list, BoundingBox* box);

// Remove a bounding box in the list freeing also the box
void list_remove(struct list* list, struct list* elm);

// Swap two elements of a list
void list_swap(struct list* l1, struct list* l2);

// Sort the list boxes by y then x
void list_sort(struct list* list, int tolerance);

// Free the whole list
void list_free_without_boxes(struct list* list);

// Free the whole list with box
void list_free_with_boxes(struct list* list);

// Hard copy of a list
struct list* list_hard_copy(struct list* list);

// Make all bounding boxes squares
void list_squarify_boxes(SDL_Surface* surface, struct list* list);

#endif /* LIST_H */
