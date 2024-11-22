#include "sauvola.h"

double k = 0.06; // Sensitivity
double w = 15.0; // Window size	
double R = 128.0; // Standard deviation

Uint8* convert_to_matrix(SDL_Surface* surface)
{
    size_t size = surface->w*surface->h;

    Uint8* res = calloc(size, sizeof(Uint8));
    for(size_t i=0; i<size; i++)
    {
	Uint32 pixel = ((Uint32*)surface->pixels)[i];
	res[i] = get_grayscale_pixel(surface, pixel);
    }
    return res;
}

float get_sauvola_threshold(double mean, double stddev, double k, double R)
{
    return mean*(1+k*((stddev/R)-1));
}

void compute_sauvola_args(int width, int height,
	int** integral, int** integral_sqrt,
	int x, int y, double* mean, double* stddev)
{
    // w = win_size
    // width = width of image/surface
    // height= height of image/surface
    int x1 = max(0, x-w/2);
    int y1 = max(0, y-w/2);

    int x2 = min(width-1, x+w/2);
    int y2 = min(height-1, y+w/2);

    int area = (x2-x1)*(y2-y1);

    double sum = (*integral)[y2*width+x2]
	+ (*integral)[y1*width+x1]
	- (*integral)[y1*width+x2]
	- (*integral)[y2*width+x1];

    double sum_sqrt = (*integral_sqrt)[y2*width+x2]
	+ (*integral_sqrt)[y1*width+x1]
	- (*integral_sqrt)[y1*width+x2]
	- (*integral_sqrt)[y2*width+x1];

    *mean = sum/(double)area;
    *stddev = sqrt((sum_sqrt/(double)area)-(*mean)*(*mean));
}

void process_sauvola(SDL_Surface* surface)
{
    int width = surface->w;
    int height = surface->h;
    int* integral = calloc(width*height, sizeof(double));

    int* integral_sqrt = calloc(width*height, sizeof(double));
    
    int* thresholds = calloc(width*height, sizeof(double));

    Uint8* input = convert_to_matrix(surface);

    // Compute initial values of integral matrix and integral sqrted matrix
    for(int j=0; j<surface->h; j++)
    {
	for(int i=0; i<surface->w; i++)
	{
	    int value = input[j*width+i];
	    integral[j*width+i] = value
			       + (i>0 ? integral[j*width+i-1] : 0)
			       + (j>0 ? integral[(j-1)*width+i] : 0)
			       - (i>0 && j>0 ? integral[(j-1)*width+i-1] : 0);

	    integral_sqrt[j*width+i] = value* value
			       + (i>0 ? integral_sqrt[j*width+i-1] : 0)
			       + (j>0 ? integral_sqrt[(j-1)*width+i] : 0)
			       - (i>0 && j>0 ? integral_sqrt[(j-1)*width+i-1] : 0);
	}
    }

    // Compute all threshold values
    for(int j=0; j<surface->h; j++)
    {
	for(int i=0; i<surface->w; i++)
	{
	    double mean;
	    double stddev;
	    compute_sauvola_args(width, height, &integral, &integral_sqrt,
		    i, j, &mean, &stddev);
	    thresholds[j*width+i] = get_sauvola_threshold(mean, stddev, k, R);
	    convert_pixel_with_threshold(surface, thresholds[j*width+i],
		    j*width+i);
//	    if(thresholds[j*width+i]<0)
//	    {
//		printf("\nwidth:%i\n",width);
//		printf("heigth:%i\n", height);
//		printf("i:%i\n",i);
//		printf("j:%i\n", j);
//		printf("%lf\n", mean);
//		printf("%lf\n", stddev);
//		printf("index:%i\n",j*width+i);
//		return;
//	    }
//	    if(thresholds[j*width+i]>0)
//		printf("%i ", thresholds[j*width+i]);
	}
    }

    free(input);
    free(integral);
    free(integral_sqrt);
    free(thresholds);
}

void replace_all_pixels(SDL_Surface* surface)
{
    process_sauvola(surface);
}







