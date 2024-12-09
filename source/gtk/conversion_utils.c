#include "conversion_utils.h"
/**
 * @brief EXPLICIT CONVERSION METHOD! => Converts a GdkPixbuf to an SDL_Surface.
 * GdkPixbuf is used in GTK for image handling whereas SDL_Surface is used in
 * SDL for rendering surfaces. Necessary to call preprocessing functions in the
 * app.
 * @param surface The surface to convert to a GdkPixbuf.
 * @return A pointer to the new GdkPixbuf.
 */
GdkPixbuf *sdl_surface_to_gdk_pixbuf(SDL_Surface *surface)
{
  // printf("🖼️ Converting SDL_Surface to GdkPixbuf\n");
  // Retrieve the surface data about its pixels
  int width = surface->w;
  int height = surface->h;
  int rowstride = surface->pitch;
  guchar *pixels = (guchar *)surface->pixels;

  // printf("📏 Width: %d, Height: %d, Rowstride: %d\n", width, height, rowstride);

  // Call gdk_pixbuf_new_from_data to create a new GdkPixbuf from the data
  // retrieved
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
      pixels,                                // data = pixels
      GDK_COLORSPACE_RGB,                    // colorspace = RGB
      surface->format->Amask ? TRUE : FALSE, // has_alpha ? Yes if Amask != 0
      8,                                     // Currently only RGB images with 8 bits per sample are supported
      width, height,
      rowstride, // nb bytes of a row's size
      NULL,      // no supported destroy
      NULL);     // no supported destroy

  // printf("✅ Conversion to GdkPixbuf done\n");
  return pixbuf;
}

/**
 * @brief EXPLICIT CONVERSION METHOD! => Converts an SDL_Surface to a GdkPixbuf.
 * GdkPixbuf is used in GTK for image handling whereas SDL_Surface is used in
 * SDL for rendering surfaces. Necessary to call preprocessing functions in the
 * app.
 * @param pixbuf The pixbuf to convert to an SDL_Surface.
 * @return A pointer to the new SDL_Surface.
 */
SDL_Surface *gdk_pixbuf_to_sdl_surface(GdkPixbuf *pixbuf)
{
  // printf("🖼️ Converting GdkPixbuf to SDL_Surface\n");
  // Retrieve the pixbuf data about its pixels
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
  int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

  // printf("📏 Width: %d, Height: %d, Rowstride: %d, Channels: %d\n", width,
         height, rowstride, n_channels);

         // Call SDL_CreateRGBSurfaceFrom to create a new SDL_Surface from the data
         // retrieved
         SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
             pixels, width, height,
             8 * n_channels, // 8 bits per pixel * number of channels
             rowstride,      // nb bytes of a row's size
             0x000000FF,     // Red mask as hexa
             0x0000FF00,     // Green mask as hexa
             0x00FF0000,     // Blue mask as hexa
             n_channels == 4
                 ? 0xFF000000
                 : 0); // Alpha mask if 4  (as hexa) channels (= RGBA) else 0 (= RGB)

         // printf("✅ Conversion to SDL_Surface done\n");
         return surface;
}

/**
 * @brief Converts a GtkImage to a GdkPixbuf.
 * Copies pixel data from a GtkImage to a new GdkPixbuf.
 * If the original pixbuf is NULL, uses the sample image.
 * @param image Pointer to the GtkImage.
 * @return New GdkPixbuf or NULL if creation failed.
 */
GdkPixbuf *image_to_pixbuf(GtkImage *image)
{
  // printf("🖼️ Converting GtkImage to GdkPixbuf\n");
  GdkPixbuf *pixbuf = NULL;
  GdkPixbuf *original_pixbuf = gtk_image_get_pixbuf(image);

  if (original_pixbuf != NULL)
  {
    int width = gdk_pixbuf_get_width(original_pixbuf);
    int height = gdk_pixbuf_get_height(original_pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(original_pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(original_pixbuf);
    guchar *pixels = gdk_pixbuf_get_pixels(original_pixbuf);

    // printf("📏 Width: %d, Height: %d, Channels: %d, Rowstride: %d\n", width,
           height, n_channels, rowstride);

           pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
           if (pixbuf == NULL)
           {
             printf("❌ New pixbuf creation failed");
             return NULL;
           }
           guchar *new_pixels =
               gdk_pixbuf_get_pixels(pixbuf); // guchar ~= unsigned char
           for (int y = 0; y < height; y++)
           {
             for (int x = 0; x < width; x++)
             {
               // Copy all of the pixel data from the original pixbuf to the new pixbuf
               guchar *src_pixel = pixels + y * rowstride + x * n_channels;
               guchar *dest_pixel =
                   new_pixels + y * gdk_pixbuf_get_rowstride(pixbuf) + x * 4;

               dest_pixel[0] = src_pixel[0]; // Red
               dest_pixel[1] = src_pixel[1]; // Green
               dest_pixel[2] = src_pixel[2]; // Blue
               dest_pixel[3] =
                   (n_channels == 4) ? src_pixel[3] : 255; // Alpha is opaque if 255
             }
           }
           // printf("✅ Conversion to new GdkPixbuf done\n");
  }
  else // If the original pixbuf is NULL, use sample image
  {
    // printf("⚠️ Original pixbuf is NULL, using sample image\n");
    pixbuf = gdk_pixbuf_new_from_file(SAMPLE_IMAGE_PATH, NULL);

    GtkImage *sample_image = GTK_IMAGE(gtk_image_new_from_pixbuf(pixbuf));
    if (sample_image == NULL)
    {
      // printf("❌ Failed to load sample image from path: %s", SAMPLE_IMAGE_PATH);
      return NULL;
    }
    //  printf("✅ Sample image loaded\n");
  }
  return pixbuf;
}