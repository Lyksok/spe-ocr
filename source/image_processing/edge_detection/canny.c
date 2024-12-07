#include "canny.h"

#define PI 3.1415926535897932 // 16 decimals
#define canny_double_threshold_low 0.3
#define canny_double_threshold_high 0.7

double *get_sobel_mask_x()
{
    double *mask = calloc(9, sizeof(double));
    double mat[9] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1};
    for (int i = 0; i < 9; i++)
        mask[i] = mat[i];
    return mask;
}

double *get_sobel_mask_y()
{
    double *mask = calloc(9, sizeof(double));
    double mat[9] = {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1};
    for (int i = 0; i < 9; i++)
        mask[i] = mat[i];
    return mask;
}

void apply_sobel_filter(SDL_Surface *surface, Uint8 **magnitude_gradient, Uint8 **direction_gradient)
{
    int len = surface->w * surface->h;
    double *sobel_mask_x = get_sobel_mask_x();
    double *sobel_mask_y = get_sobel_mask_y();

    Uint8 *x_gradient = apply_mask(surface, sobel_mask_x, 3);
    Uint8 *y_gradient = apply_mask(surface, sobel_mask_y, 3);
    free(sobel_mask_y);
    free(sobel_mask_x);

    for (int j = 0; j < surface->h; j++)
    {
        for (int i = 0; i < surface->w; i++)
        {
            Uint8 pixel_x = x_gradient[j * surface->w + i];
            Uint8 pixel_y = y_gradient[j * surface->w + i];
            double magnitude = sqrt(pixel_x * pixel_x + pixel_y * pixel_y); // Euclidean distance formula
            double direction = atan2(pixel_y, pixel_x);                     // Gradient direction

            (*magnitude_gradient)[j * surface->w + i] = (Uint8)fmin(magnitude, 255);
            (*direction_gradient)[j * surface->w + i] = (Uint8)fmin((direction + PI) / (2 * PI) * 255, 255);
        }
    }

    free(x_gradient);
    free(y_gradient);
}

void canny_edge_detection(SDL_Surface *surface, struct parameters *param)
{
    // STEP 1 : Grayscale image
    image_to_grayscale(surface, param);

    // STEP 2 : Gaussian blur
    int window_gaussian;
    double *gaussian_mask = create_gaussian_mask_5x5(&window_gaussian);
    Uint8 *blurred_image = apply_mask(surface, gaussian_mask, window_gaussian);
    free(gaussian_mask); // The caller is responsible for freeing the mask

    // STEP 3 : Sobel filter for gradient magnitude
    int len = surface->w * surface->h;
    Uint8 *magnitude_gradient = calloc(len, sizeof(Uint8));
    Uint8 *direction_gradient = calloc(len, sizeof(Uint8));

    apply_sobel_filter(surface, &magnitude_gradient, &direction_gradient);

    // STEP 4 : Non-maximum suppression
    non_maximum_suppression(surface, &magnitude_gradient, &direction_gradient);

    // STEP 5 : Double threshold for edge tracking
    double_threshold(surface, &magnitude_gradient);

    // STEP 6 : Edge tracking by hysteresis
    edge_tracking_by_hysteresis(surface, &magnitude_gradient);

    // Copy the result to the surface
    for (int j = 0; j < surface->h; j++)
    {
        for (int i = 0; i < surface->w; i++)
        {
            set_gpixel_from_coord(surface, i, j, magnitude_gradient[j * surface->w + i]);
        }
    }

    free(magnitude_gradient);
    free(direction_gradient);
}