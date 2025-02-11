#include "converting.h"

static int grayscale_method = 0;

/* Convert an image using an external function to reassign
 * each pixel with a different value based on the function
 */
void convert_image(SDL_Surface *surface,
                   Uint32 (*func)(SDL_PixelFormat *, Uint32)) {
  for (int j = 0; j < surface->h; j++) {
    for (int i = 0; i < surface->w; i++) {
      SDL_LockSurface(surface);
      Uint32 pixel = ((Uint32 *)surface->pixels)[j * surface->w + i];

      pixel = func(surface->format, pixel);
      ((Uint32 *)surface->pixels)[j * surface->w + i] = pixel;

      SDL_UnlockSurface(surface);
    }
  }
}

/* Convert an image using an external function to reassign
 * each pixel with a different value based on the function
 * Add a threshold in parameters
 */
void convert_image_with_threshold(SDL_Surface *surface, size_t threshold,
                                  Uint32 (*func)(SDL_PixelFormat *, Uint32,
                                                 size_t)) {
  for (int j = 0; j < surface->h; j++) {
    for (int i = 0; i < surface->w; i++) {
      SDL_LockSurface(surface);
      Uint32 pixel = ((Uint32 *)surface->pixels)[j * surface->w + i];

      pixel = func(surface->format, pixel, threshold);
      ((Uint32 *)surface->pixels)[j * surface->w + i] = pixel;

      SDL_UnlockSurface(surface);
    }
  }
}

/* Convert a single pixel of an image using a threshold and an index
 */
void convert_pixel_with_threshold(SDL_Surface *surface, int threshold,
                                  size_t i) {
  SDL_LockSurface(surface);
  Uint32 pixel = ((Uint32 *)surface->pixels)[i];
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
  SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
  if (r < threshold)
    pixel = SDL_MapRGBA(surface->format, 0, 0, 0, a);
  else
    pixel = SDL_MapRGBA(surface->format, 255, 255, 255, a);
  ((Uint32 *)surface->pixels)[i] = pixel;
  SDL_UnlockSurface(surface);
}

/* Map a pixel to its grayscale format
 */
Uint32 to_grayscale(SDL_PixelFormat *format, Uint32 pixel) {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
  SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
  Uint8 max = r;
  if (g > max)
    max = g;
  else {
    if (b > max)
      max = b;
  }
  Uint8 min = r;
  if (g < max)
    min = g;
  else {
    if (b < min)
      min = b;
  }
  Uint8 grayscale;
  if (grayscale_method == 1)
    grayscale = (min + max) / 2;
  else if (grayscale_method == 2)
    grayscale = (r + g + b) / 3;
  else
    grayscale = 0.299 * r + 0.587 * g + 0.114 * b;

  return SDL_MapRGBA(format, grayscale, grayscale, grayscale, a);
}

/* Convert in place an image using its surface to a grayscale image
 */
void image_to_grayscale(SDL_Surface *surface, struct parameters *param) {
  grayscale_method = param->grayscale_m;
  convert_image(surface, to_grayscale);
}

/* Map a grayscale pixel to its binarized format based
        on a threshold
 */
Uint32 to_binarized(SDL_PixelFormat *format, Uint32 pixel, size_t threshold) {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
  SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
  if (r < threshold)
    return SDL_MapRGBA(format, 0, 0, 0, a);
  else
    return SDL_MapRGBA(format, 255, 255, 255, a);
}

/* Convert in place an image using its surface to a grayscale image
 */
void image_to_binarized(SDL_Surface *surface, size_t threshold) {
  convert_image_with_threshold(surface, threshold, to_binarized);
}

void binary_histogram(int **histogram, Uint32 pixel, SDL_Surface *surface) {
  Uint8 h_index;
  Uint8 tmp1;
  Uint8 tmp2;
  SDL_GetRGB(pixel, surface->format, &h_index, &tmp1, &tmp2);
  if (h_index == 0) {
    (*histogram)[1] += 1;
  } else {
    (*histogram)[0] += 1;
  }
}

int is_inverted(SDL_Surface *surface) {
  int *histogram = malloc(2 * sizeof(int));
  histogram[0] = 0;
  histogram[1] = 0;
  create_histogram_of_pixel(surface, &histogram, binary_histogram);
  if (histogram[0] > histogram[1]) {
    free(histogram);
    return 0;
  } else {
    free(histogram);
    return 1;
  }
}

void invert_binarized_colors(SDL_Surface *surface) {
  if (!is_inverted(surface)) {
    for (int j = 0; j < surface->h; j++) {
      for (int i = 0; i < surface->w; i++) {
        SDL_LockSurface(surface);
        Uint32 pixel = ((Uint32 *)surface->pixels)[j * surface->w + i];

        Uint8 h_index;
        Uint8 tmp1;
        Uint8 tmp2;
        Uint8 a;
        SDL_GetRGBA(pixel, surface->format, &h_index, &tmp1, &tmp2, &a);
        if (h_index == 0) {
          pixel = SDL_MapRGBA(surface->format, 255, 255, 255, a);
        } else {
          pixel = SDL_MapRGBA(surface->format, 0, 0, 0, a);
        }

        ((Uint32 *)surface->pixels)[j * surface->w + i] = pixel;

        SDL_UnlockSurface(surface);
      }
    }
  }
}

static int xdir[6] = {0, 1, 1, 0, -1, -1};
static int ydir[6] = {-1, 0, 1, 1, 0, -1};

void aux_remove_box(SDL_Surface *surface, int x, int y) {
  if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
    return;
  if (get_gpixel_from_coord(surface, x, y)) {
    set_gpixel_from_coord(surface, x, y, 0);
    for (int i = 0; i < 6; i++) {
      aux_remove_box(surface, x + xdir[i], y + ydir[i]);
    }
  }
}

void remove_box_pixels(SDL_Surface *surface, BoundingBox *box) {
  aux_remove_box(surface, box->start.x, box->start.y);
}
