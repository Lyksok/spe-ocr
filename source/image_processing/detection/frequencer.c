#include "frequencer.h"

void freq_box_nb(SDL_Surface* surface, struct list* box_list,              
        int** nb_box_per_col, int** nb_box_per_row)                             
{                                                                               
    *nb_box_per_col = calloc(surface->w, sizeof(int));                            
    *nb_box_per_row = calloc(surface->h, sizeof(int));                            

    // For each box add + on row and col where we can find it
    for(struct list* p=box_list->next; p!=NULL; p=p->next)
    {
        int x0 = p->box->p1.x;
        int xf = p->box->p2.x;
        int y0 = p->box->p1.y;
        int yf = p->box->p2.y;
        for(int i=x0; i<=xf; i++)
            (*nb_box_per_col)[i]++;
        for(int i=y0; i<=yf; i++)
            (*nb_box_per_row)[i]++;
    }
}  

int freq_most_frequent(int* freq_list, int size)
{
    // Get the largest element
    int t_max = 0;
    for(int i=0; i<size; i++)
        if(t_max<freq_list[i])
            t_max = freq_list[i];

    // Create and compute the frequence of frequence
    t_max++;
    int* freq = calloc(t_max, sizeof(int));
    for(int i=0; i<size; i++)
    {
        freq[freq_list[i]]++;
    }

    // Compute the largest frequence of frequence
    int res = 2;
    for(int i=2; i<t_max; i++)
    {
        //printf("freq[%i]=%i\n",i, freq[i]);
        if(freq[res]<freq[i])
            res = i;
    }
    free(freq);
    return res;
}

int freq_get_median_from_array(int len, int* values)
{
    // Sort the array
    sort_array(&values, len, compare_ascending);
    return values[len/2];
}

int dist2(Point p1, Point p2)
{
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y - p2.y)*(p1.y - p2.y);
}

int freq_compute_minimal_distance(struct list* box, struct list* box_list)
{
    Point p1 = box_get_center(box->box);
    Point p2;
    int distance;
    int minimal_distance = -1;
    for(struct list* p=box_list->next; p!=NULL; p=p->next)
    {
        if(p==box)
            continue;
        p2 = box_get_center(p->box);

        distance = dist2(p1, p2);
        if(minimal_distance < 0 || minimal_distance > distance)
            minimal_distance = distance;
    }
    return minimal_distance;
}
