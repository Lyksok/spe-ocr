#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "detection.h"
#include "structures.h"

int add_box(BoundingBox box, BoundingBox*** boxes, int len)
{
    BoundingBox* new_box = malloc(sizeof(BoundingBox));
    *new_box = box;
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
        free(boxes[i]);
    }
    free(boxes);
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

int is_minimal(BoundingBox** boxes, int len)
{
    for(int i=0; i<len; i++)
    {
        if(boxes[i]==NULL)
            return 0;
    }
    return 1;
}

int get_nb_of_boxes(BoundingBox** boxes, int len)
{
    int res = 0;
    for(int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
            res++;
    }
    return res;
}

BoundingBox** get_minimal(BoundingBox** boxes, int len, int* res_len)
{
    *res_len = get_nb_of_boxes(boxes, len);
    BoundingBox** res = malloc(*res_len*sizeof(BoundingBox*));
    for(int i=0; i<*res_len; i++)
    {
        res[i]=NULL;
    }
    int c=0;
    for(int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
        {
            res[c]=boxes[i];
            c++;
        }
    }
    return res;
}

int average_box_height(BoundingBox** boxes, int len)
{
    int sum = 0;
    int count = 0;
    for(int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
        {
            sum += boxes[i]->p2.y - boxes[i]->p1.y;
            count++;
        }
    }
    return sum/count;
}

int average_box_width(BoundingBox** boxes, int len)
{
    int sum = 0;
    int count = 0;
    for(int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
        {
            sum += boxes[i]->p2.x - boxes[i]->p1.x;
            count++;
        }
    }
    return sum/count;
}

int is_in_box(BoundingBox* box, Point point)
{
    if(box!=NULL)
    {
        // x axis
        if(box->p2.x>=point.x && box->p1.x<=point.x)
        {
            // y axis
            if(box->p2.y>=point.y && box->p1.y<=point.y)
            {
                return 1;
            }
        }
    }
    return 0;
}

int is_in_boxes(BoundingBox** boxes, int len, Point point)
{
    for (int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
        {
            BoundingBox* box = boxes[i];
            if(is_in_box(box, point)==1)
            {
                return 1;
            }
        }
    }
    return 0;
}

Point* bounding_box_to_points(BoundingBox** boxes, int len)
{
    Point* points = malloc(len*sizeof(Point));
    for(int i=0; i<len; i++)
    {
        if(boxes[i]!=NULL)
        {
            points[i]=get_bounding_box_center(boxes[i]);
        }
    }
    return points;
}
