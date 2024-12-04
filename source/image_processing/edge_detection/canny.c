#include "canny.h"

void canny_edge_detection(SDL_Surface *surface, double threshold_low, double threshold_high)
{
    // STEP 1 : Grayscale image
    convert_image_to_grayscale(surface);
    // STEP 2 : Gaussian blur
    int window_gaussian;
    double *gaussian_mask = create_gaussian_mask_5x5(&window_gaussian);
    convolve_surface(surface, gaussian_mask, window_gaussian);
    free(gaussian_mask); // The caller is responsible for freeing the mask
    // STEP 3 : Sobel filter for gradient magnitude
    // TODO
    // STEP 4 : Non-maximum suppression
    // TODO
    // STEP 5 : Double threshold for edge tracking
    // TODO
    // STEP 6 : Edge tracking by hysteresis
    // TODO
}