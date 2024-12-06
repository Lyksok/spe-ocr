#include "pretreatment_utils.h"
/***************************************************************
 *   MULTIPURPOSE BUTTON "RUN" FOR PIPELINE DEMONSTRATION
 ***************************************************************/
void on_run_clicked(GtkWidget *button, gpointer data)
{
  GtkWidget *image_widget = GTK_WIDGET(data);
  if (!image_widget)
  {
    printf("❌ Error: image_widget is NULL\n");
    return;
  }
  printf("🚀 Running the image processing pipeline\n");

  // Convert to grayscale
  printf("🌗 Converting to grayscale\n");
  on_grayscale_clicked(button, image_widget);

  // Binarize the image
  printf("🔳 Binarizing the image\n");
  on_binarize_clicked(button, image_widget);

  // Invert colors
  printf("🔄 Inverting colors\n");
  on_invert_colors_clicked(button, image_widget);

  printf("✅ Image processing pipeline completed\n");
}
/***************************************************************
 *   ESSENTIAL PRETREATMENT FUNCTIONS
 *            shown in defense 1     *
 ***************************************************************/

// Boolean to check if the image is binarized
// Static bcs only needed here
static int is_binarized = 0; // 0 == FALSE

/**
 * @brief Callback function to convert the image to grayscaled.
 *  @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 * @note The surface is freed but the pixbuf is NOT
 */
void on_grayscale_clicked(GtkWidget *widget, gpointer data)
{
  printf("🌗 Converting to grayscale\n");
  (void)widget; // Remove unused parameter warning

  // Check if the data is a valid GtkImage
  GtkImage *image = GTK_IMAGE(data);
  if (!GTK_IS_IMAGE(data))
  {
    printf("Error: data is not a valid GtkImage\n");
    return;
  }

  // Get the GdkPixbuf from the image widget
  GdkPixbuf *pixbuf = image_to_pixbuf(image);
  if (!pixbuf)
  {
    printf("Failed to get pixbuf from image widget\n");
    return;
  }

  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  convert_to_grayscale(surface, &param);
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
void on_binarize_clicked(GtkWidget *widget, gpointer data)
{
  printf("⚫ Binarizing the image\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  convert_to_binarized_average(surface, &param);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  is_binarized = 1; // 1 == TRUE
  printf("✅ Binarization done\n");
  SDL_FreeSurface(surface); // Free the surface
}

/** TODO Dupe of the original function for debug purposes */
void invert_binarized_colors_dupe(SDL_Surface *surface)
{
  if (is_inverted(surface))
  {
    for (int j = 0; j < surface->h; j++)
    {
      for (int i = 0; i < surface->w; i++)
      {
        SDL_LockSurface(surface);
        Uint32 pixel = ((Uint32 *)surface->pixels)[j * surface->w + i];

        Uint8 h_index;
        Uint8 tmp1;
        Uint8 tmp2;
        Uint8 a;
        SDL_GetRGBA(pixel, surface->format, &h_index, &tmp1, &tmp2, &a);
        if (h_index == 0)
        {
          pixel = SDL_MapRGBA(surface->format, 255, 255, 255, a);
        }
        else
        {
          pixel = SDL_MapRGBA(surface->format, 0, 0, 0, a);
        }

        ((Uint32 *)surface->pixels)[j * surface->w + i] = pixel;

        SDL_UnlockSurface(surface);
      }
    }
  }
}

/**
 * @brief Callback function to invert the colors of the image.
 *  @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 */
void on_invert_colors_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget; // Remove unused parameter warning
  if (!is_binarized)
  {
    printf("⚠️ Invert colors can only be applied after binarizing the "
           "image.\n");
    return;
  }
  else
  {
    printf("🔄 Inverting colors\n");
    GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
    SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
    invert_binarized_colors_dupe(surface);
    GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
    display_pixbuf(data, new_pixbuf);
    printf("✅ Inverting colors done\n");
    SDL_FreeSurface(surface); // Free the surface
  }
}
/***************************************************************
 *   EXTRA DEMONSTRATION PRETREATMENT FUNCTIONS
 *            for complete demonstration of research    *
 ***************************************************************/

void on_contrast_amplification_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔧 Applying contrast amplification\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  contrast_surface(surface);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Contrast amplification done\n");
  SDL_FreeSurface(surface); // Free the surface
}

void on_median_filter_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔧 Applying median filter\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  median_filter(surface, &param);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Median filter done\n");
  SDL_FreeSurface(surface); // Free the surface
}

void on_gaussian_filter_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔧 Applying gaussian filter\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  int w;
  double *mask = create_gaussian_mask_5x5(&w);
  convolve_surface(surface, mask, w);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Gaussian filter done\n");
  SDL_FreeSurface(surface); // Free the surface
  free(mask);               // Free the mask
}

void on_average_thresholding_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔧 Applying global thresholding\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  if (pixbuf == NULL)
  {
    printf("❌ Failed to get pixbuf\n");
    return;
  }
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  if (surface == NULL)
  {
    printf("❌ Failed to convert pixbuf to SDL surface\n");
    return;
  }

  // Allocate and initialize the histogram array
  int *histogram = calloc(256, sizeof(int));
  if (histogram == NULL)
  {
    fprintf(stderr, "Error: Failed to allocate memory for histogram\n");
    SDL_FreeSurface(surface);
    return;
  }

  // Create the histogram of pixel values
  create_histogram_of_pixel(surface, &histogram, count_by_pixel);

  // Perform global thresholding using the histogram
  convert_to_binarized_global(surface, &param);

  // Free the histogram array
  free(histogram);

  // Convert the SDL surface back to a GdkPixbuf
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  if (new_pixbuf == NULL)
  {
    printf("❌ Failed to convert SDL surface to pixbuf\n");
    SDL_FreeSurface(surface);
    return;
  }

  // Display the new pixbuf
  display_pixbuf(data, new_pixbuf);
  printf("✅ Global thresholding done\n");
  SDL_FreeSurface(surface); // Free the surface
}

void on_adaptive_thresholding_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔧 Applying adaptive thresholding\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  convert_to_binarized_adaptative(surface, &param);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Adaptive thresholding done\n");
  SDL_FreeSurface(surface); // Free the surface
}

void on_otsu_thresholding_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔧 Applying Otsu thresholding\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  convert_to_binarized_global(surface, &param);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Otsu thresholding done\n");
  SDL_FreeSurface(surface); // Free the surface
}

void on_bw_gaussian_filter_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔧 Applying B&W gaussian filter\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  int w;
  double *mask = create_gaussian_mask_5x5(&w);
  convolve_surface(surface, mask, w);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  printf("✅ B&W gaussian filter done\n");
  SDL_FreeSurface(surface); // Free the surface
  free(mask);               // Free the mask
}

void on_canny_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔧 Applying Canny edge detection\n");
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  canny_edge_detection(surface, &param);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Canny edge detection done\n");
  SDL_FreeSurface(surface); // Free the surface
}