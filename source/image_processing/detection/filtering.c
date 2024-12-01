#include "filtering.h"

static int t1 = 5;
static int t2 = 300;

void filter_wrong_size_boxes(struct list* box_list)
{
    // Frequency analysis on height and width of boxes (histograms)
    // Get two higher peeks in both height and width
    // Remove all boxes that are not in the height or width peeks
}

void filter_wrong_size_boxes_threshold(struct list* box_list)
{
    printf("len=%zu\n", box_list->len);
    for(struct list* p=box_list; p->next!=NULL; p=p->next)
    {
        BoundingBox* box = p->next->box;
        int height = box_get_height(box);
        int width = box_get_width(box);
        printf("h=%i w=%i\n", height, width);
        if((height < t1 && width < t1) || (height > t2 && width > t2))
            list_remove(box_list, p->next);
    }
}
