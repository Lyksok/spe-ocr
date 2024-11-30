#include "point.h"

int point_are_equal(Point p1, Point p2)
{
    if(p1.x == p2.x && p1.y == p2.y)
        return 1;
    else
        return 0;
}

int point_is_in_box(Point point, BoundingBox* box)
{
    if (box->p2.x >= point.x && box->p1.x <= point.x)
    {
        if (box->p2.y >= point.y && box->p1.y <= point.y)
        {
            return 1;
        }
    }
    return 0;
}

int point_is_in_box_list(Point point, struct list* list)
{
    for(struct list* p=list->next; p!=NULL; p=p->next)
    {
        if(point_is_in_box(point, p->box))
            return 1;
    }
    return 0;
}

