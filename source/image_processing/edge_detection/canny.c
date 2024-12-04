#include "canny.h"
#define PI 3.1415926535897932 // 16 decimals

static double *get_sobel_mask_x()
{
    double *mask = calloc(9, sizeof(double));
    double *mat = {
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
    double *mat = {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1};
    for (int i = 0; i < 9; i++)
        mask[i] = mat[i];
    return mask;
}

void non_maximum_suppression(SDL_Surface *magnitude, SDL_Surface *direction)
{
    // Get the width and height of the image
    int width = magnitude->w;
    int height = magnitude->h;
    SDL_Surface *suppressed = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0); // Create a dsdl surface to store the suppressed image

    for (int y = 1; y < height - 1; y++)
    { // Loop through each row of the image, excluding the borders
        for (int x = 1; x < width - 1; x++)
        {
            // Loop through each column of the image, excluding the borders
            Uint8 mag = get_gpixel_from_coord(magnitude, x, y);                        // Get the gradient magnitude for the current pixel
            double dir = (get_gpixel_from_coord(direction, x, y) * 2 * PI / 255) - PI; // Get the gradient direction for the current pixel and convert it to radi

            /** By initializing neighbour1 and neighbour2 to 255, we ensure that if the gradient direction does not match any of the specified ranges, the current pixel will be suppressed because its magnitude will not be greater than or equal to 255.*/
            Uint8 neighbour1 = 255;
            Uint8 neighbour2 = 255;

            // Determine the neighboring pixels to compare based on the gradient direction
            if ((0 <= dir && dir < PI / 8) || (7 * PI / 8 <= dir && dir <= PI))
            {
                neighbour1 = get_gpixel_from_coord(magnitude, x, y + 1); // Get the pixel value to the right
                neighbour2 = get_gpixel_from_coord(magnitude, x, y - 1); // Get the pixel value to the left
            }
            else if (PI / 8 <= dir && dir < 3 * PI / 8)
            {
                neighbour1 = get_gpixel_from_coord(magnitude, x + 1, y - 1); // Get the pixel value to the top-right
                neighbour2 = get_gpixel_from_coord(magnitude, x - 1, y + 1); // Get the pixel value to the bottom-left
            }
            else if (3 * PI / 8 <= dir && dir < 5 * PI / 8)
            {
                neighbour1 = get_gpixel_from_coord(magnitude, x + 1, y); // Get the pixel value to the top
                neighbour2 = get_gpixel_from_coord(magnitude, x - 1, y); // Get the pixel value to the bottom
            }
            else if (5 * PI / 8 <= dir && dir < 7 * PI / 8)
            {
                neighbour1 = get_gpixel_from_coord(magnitude, x - 1, y - 1); // Get the pixel value to the top-left
                neighbour2 = get_gpixel_from_coord(magnitude, x + 1, y + 1); // Get the pixel value to the bottom-right
            }

            // Suppress the non-maximum pixels
            if (mag >= neighbour1 && mag >= neighbour2)
            {
                set_gpixel_from_coord(suppressed, x, y, mag); // Keep tur pixel if = current max
            }
            else
            {
                set_gpixel_from_coord(suppressed, x, y, 0); // else suppress cur pixel
            }
        }
    }
    // Copy suppressed surface => original magnitude surface
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 pixel = get_gpixel_from_coord(suppressed, x, y);
            set_gpixel_from_coord(magnitude, x, y, pixel);
        }
    }
    SDL_FreeSurface(suppressed);
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
    /**
     * Non maximum suppression without interpolation requires us to divide the 3x3 grid of pixels into 8 sections. Ie. if the gradient direction falls in between the angle -22.5 and 22.5, then we use the pixels that fall between this angle (r and q) as the value to compare with pixel p
     */
    non_maximum_suppression(magnitude_gradient, direction_gradient);
    // STEP 5 : Double threshold for edge tracking
    // TODO
    // STEP 6 : Edge tracking by hysteresis
    // TODO

    SDL_FreeSurface(gradient_x);
    SDL_FreeSurface(gradient_y);
    SDL_FreeSurface(magnitude_gradient);
    SDL_FreeSurface(direction_gradient);
}