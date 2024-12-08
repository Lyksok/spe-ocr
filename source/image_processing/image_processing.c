#include "image_processing.h"
#include "detection/list.h"

static int pixel_filter_dir_x[] = {0, 0, -1, 1}; // (int*)
static int pixel_filter_dir_y[] = {-1, 1, 0, 0}; // (int*)

struct parameters param = {
    // grayscale
    .grayscale_m = 0, // (int) 0=human, 1=min-max, 2=average

    // tolerance for list sorting
    .list_tolerance = 5, // (int)

    /* DENOISING */
    // denoising.c
    .denoising_w = 3, // (int) window

    // pixel_filter.c
    .pixel_filter_max = 20,                   // (int)
    .pixel_filter_dir_nb = 4,                 // (int)
    .pixel_filter_dir_x = pixel_filter_dir_x, // (int*)
    .pixel_filter_dir_y = pixel_filter_dir_y, // (int*)

    // erosion.c
    .erosion_m = 0, // (int) 0=cross, 1=full

    // morphology
    .dilation_m = 0, // (int) 0=cross, 1=full

    // filtering.c
    .filtering_t1 = 10,  //         (int)	      Threshold min
    .filtering_t2 = 100, //         (int)	      Threshold max
    .filtering_n = 20,   // (int)    Threshold for pixel number

    // gaussian_blur.c
    .gaussian_w = 3,

    /* THRESHOLDING */
    // average
    .average_t = 128, // (int)    threshold

    // adaptative.c
    .adaptative_w = 7, // (size_t)	Kernel size

    // sauvola.c
    .sauvola_k = 0.02,  // (double) Sensitivity
    .sauvola_w = 25.0,  // (double) Window size
    .sauvola_R = 128.0, // (double) Standard deviation

    /* UTILS */
    // sdl_utils.c
    .sdl_utils_p = 0, // (int) box padding
};

struct parameters *get_parameters() {
  return &param;
}

/* Reset all pixels of surface to a saved state of pixels */
void reset_surface(SDL_Surface *surface, Uint32 *pixels) {
  set_pixels_on_surface(surface, pixels);
}

/* Main procedure for image processing and detection */
void start_procedure(SDL_Surface *surface) {
  // Grayscale
  image_to_grayscale(surface, &param);

  // Try sauvola
  sauvola_thresholding(surface, &param);

  // Result structures
  struct list *box_list;

  // Init result structures
  box_list = list_new_list();
  init_list(box_list);

  // Remove hard noise cases and set the remaining boxes to char_list
  filter_wrong_size_boxes_threshold(surface, box_list, &param);
  list_free_with_boxes(box_list);
}
