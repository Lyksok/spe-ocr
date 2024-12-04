#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

typedef struct
{
    int x;
    int y;
} Point;

typedef struct BoundingBox
{
    Point p1;
    Point p2;
    long id;
    Point start;
    int score;
} BoundingBox;

struct list
{
    struct list* next;
    struct list* prev;
    struct BoundingBox* box;
    size_t len;
};

#endif /* STRUCTURES_H */
