#include "detection.h"

// Compute all bounding_boxes

/* Detects characters in the grid by filtering boxes
 * using their height and their width to find non-characters
 * Returns a new list with characters' boxes
 */
struct list* detect_characters(SDL_Surface* surface, struct list* box_list)
{
    filter_wrong_size_boxes_threshold(surface, box_list);
    return list_hard_copy(box_list);
}

// Auxiliary function that marks the boxes depending if they are probably in
// the grid or not
// Return the number of marked boxes
int _mark_boxes(struct list* box_list, int most_frequent_per_col,
        int most_frequent_per_row)
{
    printf("most_frequent_per_col=%i\n", most_frequent_per_col);
    printf("most_frequent_per_row=%i\n", most_frequent_per_row);
    int res=0;
    // For each box, check if it can be an element of the grid
    for(struct list* p=box_list->next; p!=NULL; p=p->next)
    {
        BoundingBox* box = p->box;
        Point center = box_get_center(box);

        // Marker list to detect grid columns
        struct list* cur_col = list_new_list();
        init_list(cur_col);

        // Check if there is enough boxes in the same column as this box
        // by counting the number of boxes that are in the column of the center
        // of the box
        int col_count=0;
        for(struct list* p2=box_list->next; p2!=NULL; p2=p2->next)
        {
            BoundingBox* box2 = p2->box;
            if(point_is_in_box_width(center, box2))
            {
                list_push_tail(cur_col, box2);
                col_count++;
            }
        }
        //printf("col_count=%i\n", col_count);

        if(col_count < most_frequent_per_col)
        {
            list_free_without_boxes(cur_col);
            continue;
        }

        // Marker list to detect grid rows
        struct list* cur_row = list_new_list();
        init_list(cur_row);

        // Check if there is enough boxes in the same row as this box
        // by counting the number of boxes that are in the row of the center
        // of the box
        int row_count=0;
        for(struct list* p2=box_list->next; p2!=NULL; p2=p2->next)
        {
            BoundingBox* box2 = p2->box;
            if(point_is_in_box_height(center, box2))
            {
                list_push_tail(cur_row, box2);
                row_count++;
            }
        }

        //printf("row_count=%i\n", row_count);
        // If there is enough boxes then mark them for later 
        if(col_count >= most_frequent_per_col
                && row_count >= most_frequent_per_row)
        {
            for(struct list* m=cur_col->next; m!=NULL; m=m->next)
            {
                if(m->box->score==0)
                    res++;
                m->box->score += 1;
            }
            for(struct list* m=cur_row->next; m!=NULL; m=m->next)
            {
                if(m->box->score==0)
                    res++;
                m->box->score += 1;
            }
        }

        list_free_without_boxes(cur_col);
        list_free_without_boxes(cur_row);
    }
    return res;
}

// Filter boxes based on new probable grid results and search for new probable
// grid results
// If the output results are the same return 0
// Else return 1
int _refine_grid_size(SDL_Surface* surface, struct list* char_list, 
        int* nb_box_per_col, int* nb_box_per_row,
        int* most_frequent_per_col, int* most_frequent_per_row)
{
    int grid_width = *most_frequent_per_col;
    int grid_height = *most_frequent_per_row;

    struct list* tmp = list_new_list();
    init_list(tmp);

    // For each box in the list, if it is not in correct grid_width then
    // don't process it
    for(struct list* p=char_list; p!=NULL && p->next!=NULL; p=p->next)
    {
        BoundingBox* box = p->next->box;
        if(nb_box_per_row[box_get_center(box).y]>=grid_width
                && nb_box_per_col[box_get_center(box).x]>=grid_height)
        {
            list_push_tail(tmp, box);
            printf("%ix%i\n", box_get_width(box),box_get_height(box));
        }
    }

    // Nb of box per col and row
    freq_box_nb(surface, tmp, &nb_box_per_col, &nb_box_per_row); 

    // Get most frequent nb of box per col and row
    *most_frequent_per_col = freq_most_frequent(nb_box_per_col, surface->w);
    *most_frequent_per_row = freq_most_frequent(nb_box_per_row, surface->h);

    if(*most_frequent_per_col==grid_width
            && *most_frequent_per_row==grid_height)
        return 0;
    else
        return 1;
}
        
/* Try to find the grid
 * If found return the grid and set its width and height
 * Else return NULL
 */
BoundingBox** detect_grid(SDL_Surface* surface, struct list* char_list,
        int* width, int* height)
{
    int tolerance = 5;
    list_squarify_boxes(surface, char_list);

    // Nb of box per col and row
    int* nb_box_per_col;
    int* nb_box_per_row;
    freq_box_nb(surface, char_list, &nb_box_per_col, &nb_box_per_row); 

    // Get most frequent nb of box per col and row
    int most_frequent_per_col = freq_most_frequent(nb_box_per_col, surface->w);
    int most_frequent_per_row = freq_most_frequent(nb_box_per_row, surface->h);

    int i=10;
    while(_refine_grid_size(surface, char_list,
                nb_box_per_col, nb_box_per_row,
                &most_frequent_per_col, &most_frequent_per_row)
            && i>=0)
    {
        // Wait until grid size stabilize
        i--;
    }
    if(i<0)
        printf("Terminated refine\n");

    // Sort the list of characters to find "lines"
    list_sort(char_list, tolerance);

    // Mark the boxes that are probably in the grid
    int marked = 
        _mark_boxes(char_list, most_frequent_per_col, most_frequent_per_row);

    int probable_nb_box = most_frequent_per_col*most_frequent_per_row;
    if(marked==probable_nb_box)
    {
        printf("Probably found the grid\n");
    }
    else
    {
        if(marked>probable_nb_box)
        {
            printf("Need more filtering to find the grid\n");
            return NULL;
        }
        else
        {
            printf("Cannot find the grid(marked=%i)\n", marked);
            return NULL;
        }
    }

    // Create the grid and initialize with 0 (NULL)
    BoundingBox** grid = calloc(most_frequent_per_col*most_frequent_per_row,
            sizeof(BoundingBox*));

    *width = most_frequent_per_row;
    *height = most_frequent_per_col;
    return grid;
}
