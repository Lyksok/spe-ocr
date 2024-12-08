#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <SDL2/SDL.h>
#include "utils/pixel_utils.h"
#include "utils/converting.h"
#include "thresholding/sauvola.h"
#include "parameters.h"
#include "denoising/gaussian_blur.h"
#include "detection/list.h"
#include "detection/bounding-box.h"
#include "detection/filtering.h"
#include "../detection-and-segmentation/cluster-detection.h"
#include "denoising/morphology.h"
#include "binarization/binarizing.h"

struct parameters* get_parameters();

#endif /* IMAGE_PROCESSING_H */
