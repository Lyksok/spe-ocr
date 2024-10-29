#include "binarizing.h"
#include "converting.h"
#include "global_thresholding.h"

void convert_to_grayscale(SDL_Surface* surface)
{
    image_to_grayscale(surface);
}

void convert_to_binarized(SDL_Surface* surface)
{
    size_t threshold = get_optimal_threshold(surface);
    image_to_binarized(surface, threshold);
}