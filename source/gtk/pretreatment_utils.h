#ifndef PRETREATMENT_UTILS_H
#define PRETREATMENT_UTILS_H

#include "main.h"
#include "../image_processing/binarization/binarizing.h"
#include "conversion_utils.h"
#include "image_utils.h"
#include "pretreatment_utils.h"
#include "rotation_utils.h"
#include "../image_processing/utils/converting.h"
#include "../image_processing/image_processing.h"

void on_grayscale_clicked(GtkWidget *widget, gpointer data);
void on_binarize_clicked(GtkWidget *widget, gpointer data);
void invert_binarized_colors_dupe(SDL_Surface *surface);
void on_invert_colors_clicked(GtkWidget *widget, gpointer data);

#endif // PRETREATMENT_UTILS_H
