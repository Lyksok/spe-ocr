#include "list.h"

static long id=0;

void init_list(struct list* list)
{
    list->next = NULL;
    list->len = 0;
}

struct list* list_new_list()
{
    struct list* elm = calloc(1, sizeof(struct list));
    elm->box = calloc(1, sizeof(BoundingBox));
    elm->box->id=id++;
    return elm;
}

int list_is_empty(struct list* list)
{
    return list->next == NULL ? 1 : 0;
}

size_t list_len(struct list *list)
{
    return list->len;
}

int list_contains(struct list* list, BoundingBox* box)
{
    for(struct list* p=list->next; p!=NULL; p=p->next)
    {
        // Check if bounding box id is different for all
        if(box_are_equal(box, p->box))
            return 1;
    }
    return 0;
}

void list_push_front(struct list* list, BoundingBox* box)
{
    if(list_contains(list, box))
        return;
    struct list* elm = list_new_list();

    // Hard copy box
    elm->box->p1=box->p1;
    elm->box->p2=box->p2;
    elm->box->start = box->start;

    // Attach the box in the front
    struct list* nxt = list->next;
    elm->next = nxt;
    elm->prev = list;
    if(nxt != NULL)
        nxt->prev = elm;
    list->next = elm;
    list->len++;
}

void list_push_tail(struct list* list, BoundingBox* box)
{
    if(list_contains(list, box))
        return;
    struct list* elm = list_new_list();

    // Hard copy box
    elm->box->p1=box->p1;
    elm->box->p2=box->p2;
    elm->box->start = box->start;

    // Attach the box on the tail
    struct list* p = list;
    while(p->next!=NULL)
        p=p->next;    
    p->next = elm;
    elm->prev = p;

    list->len++;
}

void list_remove(struct list* list, struct list* elm)
{
    list->len--;
    elm->prev->next = elm->next;
    if(elm->next != NULL)
        elm->next->prev = elm->prev;
    free(elm->box);
    free(elm);
}

void list_swap(struct list* l1, struct list* l2)
{
    l1->next=l2->next;
    l2->next=l1;
    l2->prev=l1->prev;
    l1->prev=l2;
}

void list_sort(struct list* list, int tolerance)
{
    if(list->len <= 1)
        return;

    int swapped;
    int len = (int)list->len;

    for(int i=0; i<len-1; i++)
    {
        struct list* p = list->next;
        swapped = 0;

        for(int j=0; j<len-i-1 && p!=NULL && p->next!=NULL; j++)
        {
            struct list* l1 = p;
            struct list* l2 = p->next;

            BoundingBox* b1 = l1->box;
            BoundingBox* b2 = l2->box;

            // Check if b2 is not greater than b1
            Point p1 = box_get_center(b1);
            Point p2 = box_get_center(b2);
            if(!(abs(p1.y-p2.y)<=tolerance && p1.x<=p2.x))
            {
                list_swap(l1, l2);
                swapped = 1;
            }
            p=p->next;
        }
        if(!swapped)
            break;
    }
}

void list_free_without_boxes(struct list* list)
{
    struct list* last=list;
    for(struct list* p=list->next; p!=NULL; p=p->next)
    {
        free(last);
        last=p;
    }
    free(last);
}

void list_free_with_boxes(struct list* list)
{
    struct list* last=list;
    for(struct list* p=list->next; p!=NULL; p=p->next)
    {
        free(last->box);
        free(last);
        last=p;
    }
    free(last->box);
    free(last);
}

struct list* list_hard_copy(struct list* list)
{
    struct list* res = calloc(1, sizeof(struct list));
    init_list(res);
    for(struct list* p=list->next; p!=NULL; p=p->next)
    {
        list_push_tail(res, p->box);
    }
    return res;
}

void list_squarify_boxes(SDL_Surface* surface, struct list* list)
{
    int width;
    int height;
    int to_add;
    for(struct list* p=list->next; p!=NULL; p=p->next)
    {
        width = box_get_width(p->box);
        height = box_get_height(p->box);
        if(width>height)
        {
            to_add = width-height;
            p->box->p1.y=max(p->box->p1.y-to_add/2,0);
            p->box->p2.y=min(p->box->p2.y+to_add/2,surface->h-1);
        }
        else if(width<height)
        {
            to_add = height-width;
            p->box->p1.x=max(p->box->p1.x-to_add/2,0);
            p->box->p2.x=min(p->box->p2.x+to_add/2,surface->w-1);
        }
    }
}

void list_reset_box_score(struct list* list)
{
    for(struct list* p=list->next; p!=NULL; p=p->next)
    {
        p->box->score = 0;
    }
}
