#ifndef LIST_H
#define LIST_H


#include <stddef.h>
#include <stdlib.h>
#include "structures.h"
#include "bounding-box.h"

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

// Insert a bounding box in the list (keeping the list sorted)
void list_insert(struct list* list, BoundingBox* box);

// Remove a bounding box in the list
void list_remove(struct list* list, struct list* elm);

// Sort the list boxes by y then x
void list_sort(struct list* list);

// Free the whole list
void list_free(struct list* list);

#endif /* LIST_H */
