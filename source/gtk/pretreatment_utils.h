#ifndef PRETREATMENT_UTILS_H
#define PRETREATMENT_UTILS_H

#include "main.h"
#include "../binarization/binarizing.h"
#include "conversion_utils.h"
#include "image_utils.h"
#include "pretreatment_utils.h"
#include "rotation_utils.h"

void on_grayscale_clicked(GtkWidget *widget, gpointer data);
void on_binarize_clicked(GtkWidget *widget, gpointer data);
void on_invert_colors_clicked(GtkWidget *widget, gpointer data);

#endif // PRETREATMENT_UTILS_H