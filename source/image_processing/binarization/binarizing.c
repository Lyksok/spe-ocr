#include "binarizing.h"

/* Convert an image to its grayscale representation
 */
void convert_to_grayscale(SDL_Surface *surface, struct parameters *param) {
  image_to_grayscale(surface, param);
}

/* Convert an image to its binarized representation
 * using 127 as a threshold
 */
void convert_to_binarized_average(SDL_Surface *surface,
                                  struct parameters *param) {

  image_to_binarized(surface, param->average_t);
}

/* Convert an image to its binarized representation
 * This function uses the global thresholding method
 */
void convert_to_binarized_global(SDL_Surface *surface,
                                 struct parameters *param) {
  global_thresholding(surface, param);
}

/* Convert an image to its binarized representation
 * This function uses the local thresholding method
 */
void convert_to_binarized_adaptative(SDL_Surface *surface,
                                     struct parameters *param) {
  adaptative_thresholding(surface, param);
}

void convert_to_binarized_sauvola(SDL_Surface *surface,
                                  struct parameters *param) {
  sauvola_thresholding(surface, param);
}

/* Invert black and white colors of a binarized image
 */
void invert_colors(SDL_Surface *surface) { invert_binarized_colors(surface); }
