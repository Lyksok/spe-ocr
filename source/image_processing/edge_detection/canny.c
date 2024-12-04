#include "canny.h"
#define PI 3.1415926535897932 // 16 decimals

static double *get_sobel_mask_x()
{
    double *mask = calloc(9, sizeof(double));
    double mat[] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1};
    for (int i = 0; i < 9; i++)
        mask[i] = mat[i];
    return mask;
}

static double *get_sobel_mask_y()
{
    double *mask = calloc(9, sizeof(double));
    double mat[] = {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1};
    for (int i = 0; i < 9; i++)
        mask[i] = mat[i];
    return mask;
}

void canny_edge_detection(SDL_Surface *surface, double threshold_low, double threshold_high)
{
    // STEP 1 : Grayscale image
    convert_image_to_grayscale(surface);
    // STEP 2 : Gaussian blur
    int window_gaussian;
    double *gaussian_mask = create_gaussian_mask_5x5(&window_gaussian);
    convolve_surface(surface, gaussian_mask, window_gaussian);
    free(gaussian_mask); // The caller is responsible for freeing the mask
                         // STEP 3 : Sobel filter for gradient magnitude
                         /**
                          * The whiter the point the higher the magnitude
                          * cf https://www.youtube.com/watch?v=lOEBsQodtEQ
                          */
    // creation of 2 new surfaces to store the Sobel gradient in x and y
    SDL_Surface *gradient_x = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0);
    SDL_Surface *gradient_y = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0);
    // create Sobel mask
    double *sobel_mask_x = get_sobel_mask_x();
    double *sobel_mask_y = get_sobel_mask_y();
    convolve_surface(gradient_x, sobel_mask_x, 3);
    convolve_surface(gradient_y, sobel_mask_y, 3);
    free(sobel_mask_y);
    free(sobel_mask_x);
    SDL_Surface *magnitude_gradient = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0); // 8 bits per pixel for 256 levels of grey
    SDL_Surface *direction_gradient = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0);
    for (int j = 0; j < surface->h; j++)
        for (int i = 0; i < surface->w; i++)
        {
            Uint8 pixel_x = get_gpixel_from_coord(gradient_x, i, j);
            Uint8 pixel_y = get_gpixel_from_coord(gradient_y, i, j);
            double magnitude = sqrt(pixel_x * pixel_x + pixel_y * pixel_y); // euclidean distance formula

            double direction = atan2(pixel_y, pixel_x); // atan2 = "arc tangent" => angle in rad of (y / x) in the range -pipi : Careful !  : The gradient direction is perpendicular to the edge direction
            set_gpixel_from_coord(magnitude_gradient, i, j, (int)fmin(magnitude, 255));
            set_gpixel_from_coord(direction_gradient, i, j, (int)fmin((direction + PI) / (2 * PI) * 255, 255));
        }
    // STEP 4 : Non-maximum suppression
    // TODO
    // STEP 5 : Double threshold for edge tracking
    // TODO
    // STEP 6 : Edge tracking by hysteresis
    // TODO
}