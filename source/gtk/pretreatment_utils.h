#ifndef PRETREATMENT_UTILS_H
#define PRETREATMENT_UTILS_H

#include "../binarization/binarizing.h"
#include "../binarization/converting.h"
#include "conversion_utils.h"
#include "image_utils.h"
#include "main.h"
#include "pretreatment_utils.h"
#include "rotation_utils.h"

void on_grayscale_clicked(GtkWidget *widget, gpointer data);
void on_binarize_clicked(GtkWidget *widget, gpointer data);
void invert_binarized_colors_dupe(SDL_Surface *surface);
void on_invert_colors_clicked(GtkWidget *widget, gpointer data);

#endif // PRETREATMENT_UTILS_H