#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <stdlib.h>
struct parameters
{
  // grayscale
  int grayscale_m;

  // list sorting tolerance
  int list_tolerance;

  /* DENOISING */
  // denoising.c
  int denoising_w;

  // pixel_filter.c
  int pixel_filter_max;
  int pixel_filter_dir_nb;
  int *pixel_filter_dir_x;
  int *pixel_filter_dir_y;

  // erosion.c
  int erosion_m;

  // morphology.c
  int dilation_m;

  // filtering.c
  int filtering_t1;
  int filtering_t2;
  int filtering_n;

  // gaussian_blur.c
  int gaussian_w;

  /* THRESHOLDING */
  // average
  int average_t; // Threshold for average

  // adaptative.c
  size_t adaptative_w;

  // sauvola.c
  double sauvola_k; // Sensitivity
  double sauvola_w; // Window size
  double sauvola_R; // Standard deviation

  /* UTILS */
  // sdl_utils.c
  int sdl_utils_p; // box padding
};

#endif /* PARAMETERS_H */
