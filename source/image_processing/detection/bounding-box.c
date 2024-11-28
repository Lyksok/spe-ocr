#include "detection.h"
#include "structures.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int box_are_equal(BoundingBox* b1, BoundingBox* b2)
{
    return b1->id==b2->id ? 1 : 0;
}




int average_box_height(BoundingBox **boxes, int len) {
  int sum = 0;
  int count = 0;
  for (int i = 0; i < len; i++) {
    if (boxes[i] != NULL) {
      sum += boxes[i]->p2.y - boxes[i]->p1.y;
      count++;
    }
  }
  return sum / count;
}

int average_box_width(BoundingBox **boxes, int len) {
  int sum = 0;
  int count = 0;
  for (int i = 0; i < len; i++) {
    if (boxes[i] != NULL) {
      sum += boxes[i]->p2.x - boxes[i]->p1.x;
      count++;
    }
  }
  return sum / count;
}

int is_in_box(BoundingBox *box, Point point) {
  if (box != NULL) {
    // x axis
    if (box->p2.x >= point.x && box->p1.x <= point.x) {
      // y axis
      if (box->p2.y >= point.y && box->p1.y <= point.y) {
        return 1;
      }
    }
  }
  return 0;
}

int is_in_boxes(BoundingBox **boxes, int len, Point point) {
  for (int i = 0; i < len; i++) {
    if (boxes[i] != NULL) {
      BoundingBox *box = boxes[i];
      if (is_in_box(box, point) == 1) {
        return 1;
      }
    }
  }
  return 0;
}

Point *bounding_box_to_points(BoundingBox **boxes, int len) {
  Point *points = malloc(len * sizeof(Point));
  for (int i = 0; i < len; i++) {
    if (boxes[i] != NULL) {
      points[i] = get_bounding_box_center(boxes[i]);
    }
  }
  return points;
}
