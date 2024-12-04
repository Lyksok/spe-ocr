#include "helper.h"

int min(int a, int b)
{
    return a>b ? b : a;
}

int max(int a, int b)
{
    return a<b ? b : a;
}

int abs(int a)
{
    return a<0 ? -a : a;
}

int compare_ascending(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

void sort_array(int** arr, int len, int (*compar)(const void *, const void*))
{
    qsort(*arr, len, sizeof(int), compare_ascending);
}
