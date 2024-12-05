#include "adaptative.h"

size_t KERNEL_SIZE;

/* Returns the gaussian weighted mean based on pixel at coordinates x y
 */
double get_weighted_mean(size_t x, size_t y, double sigma) {
  double xf = (double)x;
  double yf = (double)y;
  return exp(-(xf * xf + yf * yf) / (2 * sigma * sigma)) /
         (2 * M_PI * sigma * sigma);
}

typedef struct kernel {
  size_t x;
  size_t y;
  Uint8 *pixels;
  double *gaussian_distribution;
} kernel;

void free_kernel(kernel *ker) {
  free(ker->pixels);
  free(ker->gaussian_distribution);
}

Uint8 get_grayscale_pixel(SDL_Surface *surface, Uint32 pixel) {
  Uint8 res;
  Uint8 g;
  Uint8 b;
  SDL_GetRGB(pixel, surface->format, &res, &g, &b);
  return res;
}

void get_kernel(SDL_Surface *surface, size_t x, size_t y, kernel *ker) {
  size_t len = KERNEL_SIZE * KERNEL_SIZE;
  Uint8 *pixels = calloc(len, sizeof(Uint8));

  // Compute kernel limits
  size_t pad = KERNEL_SIZE / 2;
  size_t xi = x - pad;
  size_t xf = x + pad + 1;
  size_t yi = y - pad;
  size_t yf = y + pad + 1;

  // Get kernel pixel values
  for (size_t j = yi; j < yf; j++) {
    for (size_t i = xi; i < xf; i++) {
      if (j > 0 && i > 0 && j < (size_t)surface->h && i < (size_t)surface->w)
        pixels[(j - yi) * KERNEL_SIZE + (i - xi)] = get_grayscale_pixel(
            surface, ((Uint32 *)surface->pixels)[j * surface->w + i]);
      else
        pixels[(j - yi) * KERNEL_SIZE + (i - xi)] = 255;
    }
  }

  // Get kernel Gaussian values
  double *gaussian = calloc(len, sizeof(double));
  double sum = 0.0;
  size_t center = KERNEL_SIZE / 2;
  double sigma = 10.0;

  // Generate Gaussian kernel
  for (size_t j = 0; j < KERNEL_SIZE; j++) {
    for (size_t i = 0; i < KERNEL_SIZE; i++) {
      double value = get_weighted_mean(i - center, j - center, sigma);
      gaussian[j * KERNEL_SIZE + i] = value;
      sum += value;
    }
  }

  // Normalize the kernel
  for (size_t i = 0; i < len; i++) {
    gaussian[i] /= sum;
  }

  ker->x = x;
  ker->y = y;
  ker->pixels = pixels;
  ker->gaussian_distribution = gaussian;
}

int get_local_threshold(kernel *ker) {
  double threshold = 0.0;
  for (size_t i = 0; i < KERNEL_SIZE * KERNEL_SIZE; i++) {
    threshold += ((double)ker->pixels[i]) * ker->gaussian_distribution[i];
  }
  return (int)threshold;
}

void print_kernel(kernel *ker) {
  printf("(%zu,%zu):\n", ker->x, ker->y);
  printf("Pixels\n");
  for (size_t i = 0; KERNEL_SIZE > i; i++) {
    for (size_t j = 0; j < KERNEL_SIZE; j++)
      printf("%d ", ker->pixels[i * KERNEL_SIZE + j]);
    printf("\n");
  }
  printf("Gaussian\n");
  for (size_t i = 0; KERNEL_SIZE > i; i++) {
    for (size_t j = 0; j < KERNEL_SIZE; j++)
      printf("%lf ", ker->gaussian_distribution[i * KERNEL_SIZE + j]);
    printf("\n");
  }
}

void map_thresholds(int **histogram, size_t x, size_t y, SDL_Surface *surface) {
  kernel ker;
  get_kernel(surface, x, y, &ker);

  // print_kernel(&ker);
  int threshold = get_local_threshold(&ker);
  // printf("%i ", threshold);
  (*histogram)[y * surface->w + x] = threshold;
  free_kernel(&ker);
}

void get_all_thresholds(SDL_Surface *surface, int **histogram) {
  create_histogram_of_surface(surface, histogram, map_thresholds);
}

void adaptative_thresholding(SDL_Surface* surface, struct parameters* param)
{
  // Set parameters
  KERNEL_SIZE = param->adaptative_w;
  
  int *histo = calloc(surface->w * surface->h, sizeof(int));
  get_all_thresholds(surface, &histo);

  for (size_t i = 0; i < (size_t)surface->w * surface->h; i++) {
    convert_pixel_with_threshold(surface, histo[i], i);
  }
  free(histo);
}