#include "binarizing.h"
#include "converting.h"
#include "global_thresholding.h"

/* Convert an image to its grayscale representation
*/
void convert_to_grayscale(SDL_Surface* surface)
{
    image_to_grayscale(surface);
}

/* Convert a grayscale image to its binarized representation
 * This method uses the global thresholding method
*/
void convert_to_binarized_global(SDL_Surface* surface)
{
    size_t threshold = get_optimal_threshold(surface);
    image_to_binarized(surface, threshold);
}