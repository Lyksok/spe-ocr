#include "image_processing.h"

/* Reset all pixels of surface to a saved state of pixels */
void reset_surface(SDL_Surface* surface, Uint32* pixels)
{
    set_pixels_on_surface(surface, pixels);
}

/* Main procedure for image processing and detection */
void start_procedure(SDL_Surface* surface, struct parameters* param)
{
    Uint32* base_pixels = get_pixels_from_surface(surface);
    Uint32* grayscale_surface;
    Uint32* sauvola_surface;

    // Grayscale
    image_to_grayscale(surface, param);
    grayscale_surface = get_pixels_from_surface(surface);

    // Try sauvola
    sauvola_thresholding(surface, param);
    sauvola_surface = get_pixels_from_surface(surface);
    // Check detection
    // If detecting exit
    // Else revert to grayscale try other methods
}
