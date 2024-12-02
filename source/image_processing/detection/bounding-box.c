#include "bounding-box.h"

int box_are_equal(BoundingBox* b1, BoundingBox* b2)
{
    return b1->id==b2->id ? 1 : 0;
}

void box_expand_coord(BoundingBox* box, int x, int y)
{
    if(x>=0)
    {
        if(box->p2.x<x)
            box->p2.x = x;
        else if(box->p1.x>x)
            box->p1.x = x;
    }
    if(y>=0)
    {
        if(box->p2.y<y)
            box->p2.y = y;
        else if(box->p1.y>y)
            box->p1.y = y;
    }
}

void box_init_coord(BoundingBox* box, int x, int y)
{
    box->p1.x=x;
    box->p2.x=x;
    box->p1.y=y;
    box->p2.y=y;
}

void box_init_coord_from_box(BoundingBox* src, BoundingBox* dst)
{
    dst->p1 = src->p1;
    dst->p2 = src->p2;
}

int box_get_height(BoundingBox* box)
{
    return box->p2.y-box->p1.y;
}

int box_get_width(BoundingBox* box)
{
    return box->p2.x-box->p1.x;
}

Point box_get_center(BoundingBox* box)
{
    int x = (box->p1.x + box->p2.x) / 2;
    int y = (box->p1.y + box->p2.y) / 2;
    Point res = {x, y};
    return res;
}
