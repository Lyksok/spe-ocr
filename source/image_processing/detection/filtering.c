#include "filtering.h"

static int t1;
static int t2;

void filter_wrong_size_boxes(SDL_Surface* surface, struct list* box_list)
{
    // Get first peek
    // Get second peek
    // Remove every box smaller than (first+second)/2
}

void filter_wrong_size_boxes_threshold(SDL_Surface* surface,
	struct list* box_list, struct parameters* param)
{
    t1=param->filtering_t1;
    t2=param->filtering_t2;
    int i=0;
    //printf("len=%zu\n", box_list->len);
    struct list* p=box_list;
    while(p!=NULL && p->next!=NULL)
    {
        BoundingBox* box = p->next->box;
        int height = box_get_height(box);
        int width = box_get_width(box);
        //printf("h=%i w=%i (%i/%zu)\n", height, width, i++, box_list->len);
        if((height < t1 && width < t1)
		|| (height > t2 || width > t2))
        {
            remove_box_pixels(surface, p->next->box);
            list_remove(box_list, p->next);
        }
        else
        {
            p=p->next;
        }
    }
}

struct list* filter_probable_grid_boxes(struct list* box_list, int wanted_nb)
{
    // Compute closest links
    // Remove boxes that are not within the median value (with error margin)
    // while the nb of boxes is above wanted_nb

    // Return the corrected box_list
}
