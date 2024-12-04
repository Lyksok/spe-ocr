#ifndef PARAMETERS_H
#define PARAMETERS_H

struct parameters
{
    // grayscale
    int grayscale_m;

    /* DENOISING */
    // denoising.c
    int denoising_w;

    // erosion.c
    int erosion_w;

    // pixel_filter.c
    int pixel_filter_max;
    int pixel_filter_dir_nb;
    int* pixel_filter_dir_x;
    int* pixel_filter_dir_y;


    // filtering.c
    int t1;
    int t2;

    /* THRESHOLDING */
    // average
    int average_t;  // Threshold for average

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
