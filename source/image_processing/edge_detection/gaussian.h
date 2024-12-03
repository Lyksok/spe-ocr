#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <math.h>
#include "../utils/pixel_utils.h"

double* create_gaussian_mask_5x5(int* window);
double* create_gaussian_mask_3x3(int* window);

#endif /* GAUSSIAN_H */
