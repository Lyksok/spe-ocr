#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>

int min(int a, int b);
int max(int a, int b);

// Sort ascending comparison function
int compare_ascending(const void* a, const void* b);

// Sort array using compar comparison function
void sort_array(int** arr, int len, int (*compar)(const void *, const void*));

#endif /* HELPER_H */
