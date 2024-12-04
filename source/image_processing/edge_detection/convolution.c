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
	    double sumR = 0.0;
	    double sumG = 0.0;
	    double sumB= 0.0;
	    for(int y=-w/2; y<=w/2; y++)
		for(int x=-w/2; x<=w/2; x++)
		{
		    Uint8 pixelR;
		    Uint8 pixelG;
		    Uint8 pixelB;
		    if(i+x<0 || i+x>=surface->w || j+y<0 || j+y>=surface->h)
		    {
			pixelR = 255;
			pixelG = 255;
			pixelB = 255;
		    }
		    else
		    {
			pixelR = get_pixelR_from_coord(surface, i+x, j+y);;
			pixelG = get_pixelG_from_coord(surface, i+x, j+y);;
			pixelB = get_pixelB_from_coord(surface, i+x, j+y);;
		    }
		    sumR += mask[(y+w/2)*w+x+w/2]*(double)pixelR;
		    sumG += mask[(y+w/2)*w+x+w/2]*(double)pixelG;
		    sumB += mask[(y+w/2)*w+x+w/2]*(double)pixelB;
		}
	    set_pixelRGB_from_coord(surface, i, j,
		(int)fmin(sumR,255), (int)fmin(sumG,255), (int)fmin(sumB,255));
	    //printf("%lf ",sum);
	}
    free(pixels);
}
