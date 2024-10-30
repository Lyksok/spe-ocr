#include "binarizing.h"

/* Convert an image to its grayscale representation
 */
void convert_to_grayscale(SDL_Surface *surface)
{
    image_to_grayscale(surface);
}

/* Convert an image to its binarized representation
 * This function uses the global thresholding method
 */
void convert_to_binarized_global(SDL_Surface *surface)
{
    image_to_grayscale(surface);
    size_t threshold = get_optimal_threshold(surface);
    image_to_binarized(surface, threshold);
}

/* Convert an image to its binarized representation
 * This function uses the local thresholding method
 */
void convert_to_binarized_local(SDL_Surface *surface)
{
    image_to_grayscale(surface);
    int *histo = calloc(surface->w * surface->h, sizeof(int));
    get_all_thresholds(surface, &histo);

    for (size_t i = 0; i < (size_t)surface->w * surface->h; i++)
    {
        convert_pixel_with_threshold(surface, histo[i], i);
    }
    free(histo);
}