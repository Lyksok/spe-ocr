#include "pretreatment_utils.h"

// Boolean to check if the image is binarized
// Static bcs only needed here
static int is_binarized = 0; // 0 == FALSE

/**
 * @brief Callback function to convert the image to grayscaled.
 *  @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 * @note The surface is freed but the pixbuf is NOT
 */
void on_grayscale_clicked(GtkWidget *widget, gpointer data) {
  printf("🌗 Converting to grayscale\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  convert_to_grayscale(surface);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Grayscale conversion done\n");
  SDL_FreeSurface(surface); // Free the surface
}

/**
 * @brief Callback function to convert the image to binarized.
 *  @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 * @note The surface is freed but the pixbuf is NOT
 */
void on_binarize_clicked(GtkWidget *widget, gpointer data) {
  printf("⚫ Binarizing the image\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  convert_to_binarized_global(surface);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  is_binarized = 1; // 1 == TRUE
  printf("✅ Binarization done\n");
  SDL_FreeSurface(surface); // Free the surface
}

/**
 * @brief Callback function to invert the colors of the image.
 *  @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 */
void on_invert_colors_clicked(GtkWidget *widget, gpointer data) {
  (void)widget; // Remove unused parameter warning
  if (!is_binarized) {
    printf("⚠️ Invert colors can only be applied after binarizing the "
           "image.\n");
    return;
  } else {
    printf("🔄 Inverting colors\n");
    GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
    SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
    invert_colors(surface);
    GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
    display_pixbuf(data, new_pixbuf);
    printf("✅ Inverting colors done\n");
    SDL_FreeSurface(surface); // Free the surface
  }
}