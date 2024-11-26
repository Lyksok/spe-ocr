#include "convolution.h"

void convolve_surface(SDL_Surface* surface, double* mask, int w)
{
    Uint8* pixels = calloc(surface->w*surface->h, sizeof(Uint8));
    for(int i=0; i<surface->w*surface->h; i++)
    {
	pixels[i]=get_gpixel_from_coord(surface, i, 0);
    }
    for(int j=0; j<surface->h; j++)
	for(int i=0; i<surface->w; i++)
	{
	    double sum = 0.0;
	    for(int y=-w/2; y<=w/2; y++)
		for(int x=-w/2; x<=w/2; x++)
		{
		    Uint8 pixel;
		    if(i+x<0 || i+x>surface->w || j+y<0 || j+y>=surface->h)
			pixel = 255;
		    else
			pixel = pixels[(j+y)*surface->w+i+x];
		    sum += mask[(y+w/2)*w+x+w/2]*(double)pixel;
		}
	    set_gpixel_from_coord(surface, i, j, (int)fmin(sum,255));
	    //printf("%lf ",sum);
	}
    free(pixels);
}
