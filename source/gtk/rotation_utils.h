#ifndef ROTATION_UTILS_H
#define ROTATION_UTILS_H

// INCLUDE DEPENDENCIES
#include "main.h"

// CONSTANTS
#define PI 3.1415926535897932
#define DEFAULT_LEFT_ANGLE -5.0 // Default left rotation angle
#define DEFAULT_RIGHT_ANGLE 5.0 // Default right rotation angle

// extern variables can be used by other files
extern double left_angle;
extern double right_angle;

GdkPixbuf *rotate_pixbuf(GdkPixbuf *pixbuf, double angle);
void on_rotate_left_clicked(GtkWidget *widget, gpointer data);
void on_rotate_right_clicked(GtkWidget *widget, gpointer data);
void on_angle_entry_activate(GtkEntry *entry, gpointer data);

#endif // ROTATION_UTILS_H