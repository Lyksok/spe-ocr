#include "binarizing.h"
#include "converting.h"
#include "global_thresholding.h"
#include "local_thresholding.h"

/* Convert an image to its grayscale representation
*/
void convert_to_grayscale(SDL_Surface* surface)
{
    image_to_grayscale(surface);
}

/* Convert an image to its binarized representation
 * using 127 as a threshold
*/
void convert_to_binarized_average(SDL_Surface* surface)
{
    image_to_grayscale(surface);
    image_to_binarized(surface, 127);
}

/* Convert an image to its binarized representation
 * This function uses the global thresholding method
*/
void convert_to_binarized_global(SDL_Surface* surface)
{
    image_to_grayscale(surface);
    size_t threshold = get_optimal_threshold(surface);
    image_to_binarized(surface, threshold);
}

/* Convert an image to its binarized representation
 * This function uses the local thresholding method
*/
void convert_to_binarized_local(SDL_Surface* surface)
{
    image_to_grayscale(surface);
    int* histo = calloc(surface->w*surface->h, sizeof(int));
    get_all_thresholds(surface, &histo);

    for(size_t i=0; i<(size_t)surface->w*surface->h; i++)
    {
        convert_pixel_with_threshold(surface, histo[i], i);
    }
    free(histo);
}