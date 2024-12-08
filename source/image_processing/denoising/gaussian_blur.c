#include "gaussian_blur.h"

static int w = 3;

static double mask_5x5[] = {0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
                            0.015019, 0.059913, 0.094907, 0.059913, 0.015019,
                            0.023792, 0.094907, 0.150343, 0.094907, 0.023792,
                            0.015019, 0.059913, 0.094907, 0.059913, 0.015019,
                            0.003765, 0.015019, 0.023792, 0.015019, 0.003765};

static double mask_3x3[] = {1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0,
                            1.0 / 8.0,  1.0 / 4.0, 1.0 / 8.0,
                            1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0};

static void convolve_surface(SDL_Surface *surface, double *mask) {
  Uint8 *pixels = get_gpixels_from_surface(surface);
  for (int j = 0; j < surface->h; j++)
    for (int i = 0; i < surface->w; i++) {
      double sumR = 0.0;
      double sumG = 0.0;
      double sumB = 0.0;
      for (int y = -w / 2; y <= w / 2; y++)
        for (int x = -w / 2; x <= w / 2; x++) {
          Uint8 pixelR;
          Uint8 pixelG;
          Uint8 pixelB;
          if (i + x < 0 || i + x >= surface->w || j + y < 0 ||
              j + y >= surface->h) {
            pixelR = 255;
            pixelG = 255;
            pixelB = 255;
          } else {
            pixelR = get_pixelR_from_coord(surface, i + x, j + y);
            ;
            pixelG = get_pixelG_from_coord(surface, i + x, j + y);
            ;
            pixelB = get_pixelB_from_coord(surface, i + x, j + y);
            ;
          }
          sumR += mask[(y + w / 2) * w + x + w / 2] * (double)pixelR;
          sumG += mask[(y + w / 2) * w + x + w / 2] * (double)pixelG;
          sumB += mask[(y + w / 2) * w + x + w / 2] * (double)pixelB;
        }
      set_pixelRGB_from_coord(surface, i, j, (int)fmin(sumR, 255),
                              (int)fmin(sumG, 255), (int)fmin(sumB, 255));
      // printf("%lf ",sum);
    }
  free(pixels);
}

void gaussian_blur(SDL_Surface *surface, struct parameters *param) {
  w = param->gaussian_w;
  double *mask = mask_3x3;
  if (w == 3)
    mask = mask_3x3;
  else if (w == 5)
    mask = mask_5x5;

  convolve_surface(surface, mask);
}
