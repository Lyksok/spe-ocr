#include "erosion.h"

static int w=3;

void erode_surface(SDL_Surface* surface, struct parameters* param)
{
	w=param->erosion_w;
    int pixel_nb = surface->w*surface->h;
    Uint8 pixels[pixel_nb] = {};
    for(int i=0; i<pixel_nb; i++)
	pixels[i]=get_gpixel_from_array(surface, i);

    for(int j=0; j<surface->h; j++)
    {
        for(int i=0; i<surface->w; i++)
        {
	    Uint8 pixel = get_gpixel_from_coord(surface, i, j);
	    int fg;
	    if(pixel)
		fg = 1;
	    else
		fg = 0;
	    /*
	    for(int y=-w/2; y<=w/2; y++)
	    {
		if(!fg)
		    break;
		for(int x=-w/2; x<=w/2; x++)
		{
		    if(i+x<0 || i+x>=surface->w || j+y<0 || j+y>=surface->h)
		    {
			fg = 0;
			break;
		    }
		    else
		    {
			if(!get_gpixel_from_coord(surface, i+x, j+y))
			{
			    fg = 0;
			    break;
			}
		    }
		}
	    }
	    */
	    if(i-1<0 || i+1>=surface->w || j-1<0 || j+1>=surface->h)
	    {
		fg = 0;
	    }
	    fg = fg && get_gpixel_from_coord(surface, i-1, j)
		    && get_gpixel_from_coord(surface, i+1, j)
		    && get_gpixel_from_coord(surface, i, j-1)
		    && get_gpixel_from_coord(surface, i, j+1);
	    if(!fg)
		pixels[j*surface->w+i]=0;
	    else
		pixels[j*surface->w+i]=pixel;
        }
    }
    for(int i=0; i<pixel_nb; i++)
	set_gpixel_from_coord(surface, i, 0, pixels[i]);
}
