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
#include "../image_processing/edge_detection/gaussian.h"
#include "../image_processing/edge_detection/convolution.h"
#include "../image_processing/denoising/erosion.h"
#include "../image_processing/denoising/pixel_filter.h"
#include "../image_processing/detection/detection.h"
#include "../image_processing/detection/cluster-detection.h"
#include "../image_processing/detection/list.h"
#include "../image_processing/utils/sdl_utils.h"
#include "../image_processing/utils/converting.h"

void on_grayscale_clicked(GtkWidget *widget, gpointer data);
void on_binarize_clicked(GtkWidget *widget, gpointer data);
void invert_binarized_colors_dupe(SDL_Surface *surface);
void on_invert_colors_clicked(GtkWidget *widget, gpointer data);

#endif // PRETREATMENT_UTILS_H
