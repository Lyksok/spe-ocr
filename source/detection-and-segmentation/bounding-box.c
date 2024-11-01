#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "structures.h"


int add_box(BoundingBox box, BoundingBox*** boxes, int len)
{
    BoundingBox* new_box = malloc(sizeof(BoundingBox));
    new_box->p1 = box.p1;
    new_box->p2 = box.p2;
    int i=0;
    while(i<len && (*boxes)[i]!=NULL)
    {
        i++;
    }
    if(i<len)
    {
        (*boxes)[i]=new_box;
        return 0;
    }
    else
        return 1;
}

int are_equal_point(Point p1, Point p2)
{
    if(p1.x == p2.x && p1.y == p2.y)
        return 1;
    else
        return 0;
}

int are_equal_box(BoundingBox b1, BoundingBox b2)
{
    if(are_equal_point(b1.p1, b2.p1)
        && are_equal_point(b1.p2, b2.p2))
        return 1;
    else
        return 0;
}

int remove_box(BoundingBox box, BoundingBox*** boxes, int len)
{
    for(int i=0; i<len; i++)
    {
        if(are_equal_box(*(*boxes)[i], box))
        {
            free((*boxes)[i]);
            (*boxes)[i]=NULL; 
            return 0;
        }
    }
    return 1;
}

int contains_box(BoundingBox box, BoundingBox** boxes, int len)
{
    for(int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
        {
            if(are_equal_box(*(boxes[i]), box))
            {
                return 1;
            }
        }
    }
    return 0;
}

void free_boxes(BoundingBox** boxes, int len)
{
    for(int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
            free(boxes[i]);
    }
}

void print_points(Point p1, Point p2)
{
    printf("p1(%i,%i) p2(%i,%i)\n",p1.x,p1.y,p2.x,p2.y);
}

void print_boxes(BoundingBox** boxes, int len)
{
    for(int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
        {
            printf("Box %i: ", i);
            print_points(boxes[i]->p1, boxes[i]->p2);
        }
    }
}
