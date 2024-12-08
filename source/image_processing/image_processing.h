#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "../detection-and-segmentation/cluster-detection.h"
#include "binarization/binarizing.h"
#include "denoising/gaussian_blur.h"
#include "denoising/morphology.h"
#include "detection/bounding-box.h"
#include "detection/filtering.h"
#include "detection/list.h"
#include "parameters.h"
#include "thresholding/sauvola.h"
#include "utils/converting.h"
#include "utils/pixel_utils.h"
#include <SDL2/SDL.h>

struct parameters *get_parameters();

#endif /* IMAGE_PROCESSING_H */
