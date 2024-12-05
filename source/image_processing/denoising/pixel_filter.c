#include "pixel_filter.h"

static int max_cluster = 15;
static int dir_nb = 4;
static int* dir_x;
static int* dir_y;

void aux_flood_fill(SDL_Surface* surface, int* pixels, int** visited,
	int x, int y, int* count)
{
    if(x<0 || y < 0 || x>surface->w || y>surface->h)
	return;
    if(!pixels[y*surface->w+x] || (*visited)[y*surface->w+x])
	return;
    *count+=1;
    if(*count>max_cluster)
	return;
    (*visited)[y*surface->w+x] = 1;
    for (size_t i = 0; i < 4; i++)
    {
	aux_flood_fill(surface, pixels, visited,
		x+dir_x[i], y+dir_y[i], count);
    }
}

int* flood_fill_count(SDL_Surface* surface, int x, int y, int* count)
{
    int pixel_nb = surface->w*surface->h;
    int* visited = calloc(pixel_nb, sizeof(int));

    Uint32* surface_pixels = (Uint32*)surface->pixels;
    int* pixels = calloc(pixel_nb, sizeof(int));
    for(int i=0; i<pixel_nb; i++)
	pixels[i] = get_gpixel_from_coord(surface, i, 0) ? 1 : 0;

    aux_flood_fill(surface, pixels, &visited, x, y, count);
    free(pixels);
    return visited;
}

void remove_cluster(SDL_Surface* surface, int* cluster)
{
    for(int j=0; j<surface->h; j++)
    {
	for(int i=0; i<surface->w; i++)
	{
	    if(cluster[j*surface->w+i])
		set_gpixel_from_coord(surface, i, j, 0);
	}
    }
}

void filter_small_clusters(SDL_Surface* surface, struct parameters* param)
{
	max_cluster = param->pixel_filter_max;
	dir_nb = param->pixel_filter_dir_nb;
	dir_x = param->pixel_filter_dir_x;
	dir_y = param->pixel_filter_dir_y;
    // Remove clusters with 15 or less pixels
    for(int j=0; j<surface->h; j++)
    {
	for(int i=0; i<surface->w; i++)
	{
	    if(get_gpixel_from_coord(surface, i, j))
	    {
		int count = 0;
		int* cluster = flood_fill_count(surface, i, j, &count);
		if(count<=max_cluster)
		    remove_cluster(surface, cluster);
		free(cluster);
	    }
	    printf("(%i,%i) ",i, j);
	}
    }
}
