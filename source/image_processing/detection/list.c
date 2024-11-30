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
    struct list* nxt = list->next;
    elm->next = nxt;
    elm->prev = list;
    if(nxt != NULL)
        nxt->prev = elm;
    list->next = elm;
    list->len += 1;
}

void list_insert(struct list* list, BoundingBox* box)
{
    // TODO
}

void list_remove(struct list* list, BoundingBox* box)
{
    // TODO
}

void list_sort(struct list* list)
{
    // TODO
}

void list_free(struct list* list)
{
    // TODO
}
