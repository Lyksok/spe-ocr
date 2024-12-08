#ifndef ROTATION_UTILS_H
#define ROTATION_UTILS_H

// INCLUDE DEPENDENCIES
// #include "main.h"
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include "conversion_utils.h"
#include "image_utils.h"
#include <SDL2/SDL.h>
#include "main.h"
#include "my_print.h"
#include <SDL2/SDL2_rotozoom.h> // plan de secours

// CONSTANTS
#define PI 3.1415926535897932
#define DEFAULT_LEFT_ANGLE 90.0  // Default left rotation angle
#define DEFAULT_RIGHT_ANGLE 90.0 // Default right rotation angle

// extern variables can be updated by other files
extern double left_angle;
extern double right_angle;

void rotate_pixbuf(GdkPixbuf *src_pixbuf, GdkPixbuf *dst_pixbuf, double angle);
void on_rotate_left_clicked(GtkWidget *widget, gpointer data);
void on_rotate_right_clicked(GtkWidget *widget, gpointer data);
void on_left_angle_entry_activate(GtkEntry *entry, gpointer data);
void on_right_angle_entry_activate(GtkEntry *entry, gpointer data);
void on_auto_rotate_clicked(GtkWidget *widget, gpointer data);

#endif // ROTATION_UTILS_H