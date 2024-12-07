#ifndef EDGE_DETECTION_H

#define EDGE_DETECTION_H
#include <SDL2/SDL.h>
#include "convolution.h"
#include "gaussian.h"
#include "../utils/converting.h"
#include "../utils/pixel_utils.h"
#include "../parameters.h"
#include <math.h>
#include <stdlib.h>
#include "canny.h"
#include "hough_transform.h"

#define PI 3.1415926535897932

double calculate_rotation_angle(int *accumulator, int acc_width, int acc_height, double theta_step);
double auto_detect_rotation_angle(SDL_Surface *surface, struct parameters *param);


#endif