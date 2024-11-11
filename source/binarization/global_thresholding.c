#include "global_thresholding.h"

/* Create a histogram for probability distribution  using a grayscale image
 * Takes in parameter the result histogram and the image surface
 */
void get_probability_distribution(SDL_Surface *surface, double **histogram) {
  int pixel_nb = surface->w * surface->h;

  // Create a new histogram to get all pixel colors
  int *histo = calloc(256, sizeof(int));
  histogram_of_pixels(surface, &histo);

  // Fill the probability distribution histogram
  double *p_histo = *histogram;
  for (size_t i = 0; i < 256; i++) {
    p_histo[i] = (double)(histo[i]) / pixel_nb;
  }
  *histogram = p_histo;
  free(histo);
}

typedef struct weights {
  double weight1;
  double weight2;
} weights;

typedef struct means {
  double mean1;
  double mean2;
} means;

void get_weight(size_t threshold, weights *w, double **p_histogram,
                double (*opt)(size_t)) {
  w->weight1 = 0;
  w->weight2 = 0;
  double *histo = *p_histogram;
  for (size_t i = 0; i < threshold; i++) {
    w->weight1 += *(histo + i) * opt(i);
  }
  for (size_t i = threshold + 1; i < 256; i++) {
    w->weight2 += *(histo + i) * opt(i);
  }
}

double return_one(size_t t) {
  (void)t;
  return 1;
}

double return_t(size_t t) { return t; }

void get_weight_and_mean(size_t threshold, weights *w, means *m,
                         double **p_histogram) {
  w->weight1 = 0;
  w->weight2 = 0;
  m->mean1 = 0;
  m->mean2 = 0;

  get_weight(threshold, w, p_histogram, return_one);
  weights mean_weight = {0, 0};
  get_weight(threshold, &mean_weight, p_histogram, return_t);

  if (w->weight1 == 0 || w->weight2 == 0) {
    m->mean1 = 0;
    m->mean2 = 0;
  } else
    m->mean1 = mean_weight.weight1 / w->weight1;
}

double get_variance(weights *w, means *m) {
  return w->weight1 * w->weight2 * (m->mean1 - m->mean2);
}

// void __print_wm_struct(weights* w, means* m)
//{
//	printf("Weights:(%lf,%lf)\nMeans:(%lf,%lf)\n",
//			w->weight1,
//			w->weight2,
//			m->mean1,
//			m->mean2);
// }

size_t get_optimal_threshold(SDL_Surface *surface) {
  double *histo = malloc(256 * sizeof(double));
  double **p_histogram = &histo;
  *p_histogram = histo;
  get_probability_distribution(surface, p_histogram);

  weights w = {0, 0};
  means m = {0, 0};

  // Get optimal threshold using variance
  double max_variance = histo[0];
  size_t max_threshold = 0;
  for (size_t t = 0; t < 256; t++) {
    get_weight_and_mean(t, &w, &m, p_histogram);
    double var = get_variance(&w, &m);
    //__print_wm_struct(&w, &m);
    if (max_variance < var) {
      max_threshold = t;
      max_variance = var;
    }
  }

  free(histo);
  return max_threshold;
}
