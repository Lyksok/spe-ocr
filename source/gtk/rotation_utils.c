#include "rotation_utils.h"

// global variables to store the changing angle values
double left_angle = DEFAULT_LEFT_ANGLE;
double right_angle = DEFAULT_RIGHT_ANGLE;

void on_left_angle_entry_activate(GtkEntry *entry, gpointer data)
{
  if (!entry || !data)
  {
    printf("Invalid GtkEntry or data pointer.");
    return;
  }

  const char *angle_text = gtk_entry_get_text(entry);
  if (!angle_text)
  {
    printf("Failed to retrieve text from GtkEntry.");
    return;
  }

  double *angle = (double *)data;
  *angle = g_ascii_strtod(angle_text, NULL);

  printf("Left angle updated to: %.2f", *angle);
}

void on_right_angle_entry_activate(GtkEntry *entry, gpointer data)
{
  double *angle = (double *)data;
  const char *angle_text = gtk_entry_get_text(entry);
  if (!angle_text)
  {
    printf("Angle text is NULL.");
    return;
  }
  *angle = g_ascii_strtod(angle_text, NULL); // convert string to double
}
/**
 * @brief Callback function to handle the "activate" signal of the GtkEntry widgets.
 * @param entry The GtkEntry widget.
 * @param data Pointer to the rotation angle macro to update.
 */
void on_angle_entry_activate(GtkEntry *entry, gpointer data)
{
  double *angle = (double *)data;
  const char *angle_text = gtk_entry_get_text(entry);
  if (!angle_text)
  {
    printf("Angle text is NULL.");
    return;
  }
  *angle = g_ascii_strtod(angle_text, NULL); // convert string to double
}

void on_rotate_left_clicked(GtkWidget *widget, gpointer data)
{
  double angle = 90.0; // Rotate by 90 degrees to the left
  printf("🔄 Rotating image to the left by %f degrees\n", angle);
  (void)widget;

  // Convert GtkImage to SDL_Surface
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);

  // Create an SDL texture from the surface
  SDL_Window *window = SDL_CreateWindow("Rotation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, surface->w, surface->h, SDL_WINDOW_HIDDEN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  // Set up the destination rectangle
  SDL_Rect dstrect = {0, 0, surface->w, surface->h};

  // Render the texture with rotation
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderClear(renderer);
  SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_NONE);
  SDL_RenderPresent(renderer);

  // Read the pixels from the renderer into a new surface
  SDL_Surface *rotated_surface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 32, SDL_PIXELFORMAT_RGBA32);
  SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, rotated_surface->pixels, rotated_surface->pitch);

  // Convert the rotated surface back to GdkPixbuf
  GdkPixbuf *rotated_pixbuf = sdl_surface_to_gdk_pixbuf(rotated_surface);

  // Update the GtkImage with the rotated pixbuf
  gtk_image_set_from_pixbuf(GTK_IMAGE(data), rotated_pixbuf);

  // Clean up
  g_object_unref(rotated_pixbuf);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_FreeSurface(surface);
  SDL_FreeSurface(rotated_surface);

  printf("✅ Left rotation done\n");
}

void on_rotate_right_clicked(GtkWidget *widget, gpointer data)
{
  double angle = -90.0; // Rotate by 90 degrees to the right
  printf("🔄 Rotating image to the right by %f degrees\n", angle);
  (void)widget;

  // Convert GtkImage to SDL_Surface
  GdkPixbuf *pixbuf = GTK_IMAGE(data);
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);

  // Create an SDL texture from the surface
  SDL_Window *window = SDL_CreateWindow("Rotation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, surface->w, surface->h, SDL_WINDOW_HIDDEN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  // Set up the destination rectangle
  SDL_Rect dstrect = {0, 0, surface->w, surface->h};

  // Render the texture with rotation
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderClear(renderer);
  SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_NONE);
  SDL_RenderPresent(renderer);

  // Read the pixels from the renderer into a new surface
  SDL_Surface *rotated_surface = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 32, SDL_PIXELFORMAT_RGBA32);
  SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, rotated_surface->pixels, rotated_surface->pitch);

  // Convert the rotated surface back to GdkPixbuf
  GdkPixbuf *rotated_pixbuf = sdl_surface_to_gdk_pixbuf(rotated_surface);

  // Update the GtkImage with the rotated pixbuf
  gtk_image_set_from_pixbuf(GTK_IMAGE(data), rotated_pixbuf);

  // Clean up
  g_object_unref(rotated_pixbuf);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_FreeSurface(surface);
  SDL_FreeSurface(rotated_surface);

  printf("✅ Right rotation done\n");
}
/**
 * @brief Rotates a pixbuf by the specified angle using bi-linear interpolation.
 * @param src_pixbuf The original pixbuf to rotate.
 * @param dst_pixbuf The destination pixbuf to store the rotated image into.
 * @param angle The angle to rotate the image by.
 */
void rotate_pixbuf(GdkPixbuf *src_pixbuf, GdkPixbuf *dst_pixbuf, double angle)
{
  printf("🔄 Rotating pixbuf by angle: %.2f\n", angle);
  int src_width = gdk_pixbuf_get_width(src_pixbuf);
  int src_height = gdk_pixbuf_get_height(src_pixbuf);
  double angle_rad = angle * PI / 180.0;
  double cos_angle = cos(angle_rad);
  double sin_angle = sin(angle_rad);
  int dst_width = (int)(fabs(src_width * cos_angle) + fabs(src_height * sin_angle));
  int dst_height = (int)(fabs(src_width * sin_angle) + fabs(src_height * cos_angle));

  int n_channels = gdk_pixbuf_get_n_channels(src_pixbuf);

  printf("📏 Source Width: %d, Source Height: %d, Destination Width: %d, "
         "Destination Height: %d, Channels: %d\n",
         src_width, src_height, dst_width, dst_height, n_channels);

  // Ensure dst_pixbuf has the correct dimensions
  if (gdk_pixbuf_get_width(dst_pixbuf) != dst_width || gdk_pixbuf_get_height(dst_pixbuf) != dst_height)
  {
    g_object_unref(dst_pixbuf);
    dst_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, gdk_pixbuf_get_has_alpha(src_pixbuf), 8, dst_width, dst_height);
  }

  guchar *src_pixels = gdk_pixbuf_get_pixels(src_pixbuf);
  guchar *dst_pixels = gdk_pixbuf_get_pixels(dst_pixbuf);
  int src_rowstride = gdk_pixbuf_get_rowstride(src_pixbuf);
  int dst_rowstride = gdk_pixbuf_get_rowstride(dst_pixbuf);

  for (int y = 0; y < dst_height; y++)
  {
    for (int x = 0; x < dst_width; x++)
    {
      double src_x = (x - dst_width / 2) * cos_angle -
                     (y - dst_height / 2) * sin_angle + src_width / 2;
      double src_y = (x - dst_width / 2) * sin_angle +
                     (y - dst_height / 2) * cos_angle + src_height / 2;

      if (src_x >= 0 && src_x < src_width - 1 && src_y >= 0 && src_y < src_height - 1)
      {
        int x1 = (int)src_x;
        int y1 = (int)src_y;
        int x2 = x1 + 1;
        int y2 = y1 + 1;

        // Ensure x2 and y2 are within bounds
        if (x2 >= src_width)
          x2 = src_width - 1;
        if (y2 >= src_height)
          y2 = src_height - 1;

        double dx = src_x - x1;
        double dy = src_y - y1;

        for (int c = 0; c < n_channels; c++)
        {
          // Top-left pixel
          guchar *p1 = src_pixels + y1 * src_rowstride + x1 * n_channels + c;
          // Top-right pixel
          guchar *p2 = src_pixels + y1 * src_rowstride + x2 * n_channels + c;
          // Bottom-left pixel
          guchar *p3 = src_pixels + y2 * src_rowstride + x1 * n_channels + c;
          // Bottom-right pixel
          guchar *p4 = src_pixels + y2 * src_rowstride + x2 * n_channels + c;

          // Bilinear interpolation
          double value = (1 - dx) * (1 - dy) * (*p1) +
                         dx * (1 - dy) * (*p2) +
                         (1 - dx) * dy * (*p3) +
                         dx * dy * (*p4);

          dst_pixels[y * dst_rowstride + x * n_channels + c] = (guchar)value;
        }
      }
      else
      {
        for (int c = 0; c < n_channels; c++)
        {
          dst_pixels[y * dst_rowstride + x * n_channels + c] = 0; // Set to black or transparent
        }
      }
    }
  }
}

void on_auto_rotate_clicked(GtkWidget *widget, gpointer data)
{
  my_print("🔄 Auto-rotating image\n");
  (void)widget; // Remove unused parameter warning

  // Convert the image to SDL_Surface
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);

  // Detect the rotation angle
  double angle = auto_detect_rotation_angle(surface);

  // Log the detected angle
  my_print("📐 Detected rotation angle: %.2f degrees\n", angle);

  // Rotate the image by the detected angle
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  GdkPixbuf *new_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  rotate_pixbuf(pixbuf, new_pixbuf, angle);
  display_pixbuf(data, new_pixbuf);

  // Clean up
  SDL_FreeSurface(surface);
  my_print("✅ Auto-rotation done\n");
}