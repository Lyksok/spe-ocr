#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct BoundingBox {
    Point p1;
    Point p2;
} BoundingBox;

#endif /* STRUCTURES_H */
