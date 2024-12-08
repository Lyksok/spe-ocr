#ifndef PRETREATMENT_UTILS_H
#define PRETREATMENT_UTILS_H

#include "main.h"
#include "conversion_utils.h"
#include "image_utils.h"
#include "pretreatment_utils.h"
#include "rotation_utils.h"
#include "../image_processing/binarization/binarizing.h"
#include "../image_processing/thresholding/sauvola.h"
#include "../image_processing/denoising/denoising.h"
#include "../image_processing/denoising/contrasting.h"
#include "../image_processing/edge_detection/canny.h"
// #include "../image_processing/denoising/erosion.h" // Excluded due to errors
#include "../image_processing/denoising/pixel_filter.h"
#include "../image_processing/detection/list.h"
#include "../image_processing/detection/structures.h"
#include "../image_processing/utils/sdl_utils.h"
#include "../image_processing/main.c" // for the parameters
#include "../image_processing/utils/converting.h"
#include "../image_processing/parameters.h"
#include "../image_processing/image_processing.h"
#include "../image_processing/utils/histograms.h"
#include "my_print.h"

// MULTIPURPOSE BUTTON "RUN" FOR PIPELINE DEMONSTRATION
void on_run_clicked(GtkWidget *button, gpointer data);
// ESSENTIAL PRETREATMENT FUNCTIONS
void on_grayscale_clicked(GtkWidget *widget, gpointer data);
void on_binarize_clicked(GtkWidget *widget, gpointer data);
void invert_binarized_colors_dupe(SDL_Surface *surface);
void on_invert_colors_clicked(GtkWidget *widget, gpointer data);
// EXTRA DEMONSTRATION PRETREATMENT FUNCTIONS
void on_contrast_amplification_clicked(GtkWidget *widget, gpointer data);
void on_median_filter_clicked(GtkWidget *widget, gpointer data);
void on_gaussian_filter_clicked(GtkWidget *widget, gpointer data);
void on_average_thresholding_clicked(GtkWidget *widget, gpointer data);
void on_adaptive_thresholding_clicked(GtkWidget *widget, gpointer data);
void on_otsu_thresholding_clicked(GtkWidget *widget, gpointer data);
void on_bw_median_filter_clicked(GtkWidget *widget, gpointer data);
void on_bw_gaussian_filter_clicked(GtkWidget *widget, gpointer data);

#endif // PRETREATMENT_UTILS_H