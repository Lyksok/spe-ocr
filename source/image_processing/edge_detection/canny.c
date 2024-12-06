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

                set_gpixel_from_coord(suppressed, x, y, mag); // Keep the pixel if = current max
            }
            else
            {
                set_gpixel_from_coord(suppressed, x, y, 0); // else suppress cur pixel
            }
        }
    }
    // Copy suppressed surface => original magnitude surface

    SDL_Rect srcRect = {0, 0, suppressed->w, suppressed->h};
    SDL_Rect dstRect = {0, 0, magnitude->w, magnitude->h};
    SDL_BlitSurface(suppressed, &srcRect, magnitude, &dstRect);

    SDL_FreeSurface(suppressed);
}

void double_threshold(SDL_Surface *magnitude)
{
    int width = magnitude->w;
    int height = magnitude->h;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 pixel = get_gpixel_from_coord(magnitude, x, y);
            if (canny_double_threshold_low > pixel) // non-edges : black, to delete
                set_gpixel_from_coord(magnitude, x, y, 0);
            else if (pixel > canny_double_threshold_high) // strong edge : white, to keep
                set_gpixel_from_coord(magnitude, x, y, 255);
            else // weak edge : in between, determined in step 6 : if connected to strong edge => strong edge else non-edge
                set_gpixel_from_coord(magnitude, x, y, 128);
        }
    }
}

int is_within_bounds(int x, int y, int width, int height)
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

void __edge_tracking_by_hysteresis(SDL_Surface *magnitude, int x, int y, int width, int height)
{
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;
            int nx = x + dx;
            int ny = y + dy;
            if (is_within_bounds(nx, ny, width, height))
            {
                Uint8 neighbor_pixel = get_gpixel_from_coord(magnitude, nx, ny);
                if (neighbor_pixel == 128)
                {
                    set_gpixel_from_coord(magnitude, nx, ny, 255);                   // Becomes to strong edge!
                    __edge_tracking_by_hysteresis(magnitude, nx, ny, width, height); // Recursive call
                }
            }
        }
    }
}
void edge_tracking_by_hysteresis(SDL_Surface *magnitude)
{
    int width = magnitude->w;
    int height = magnitude->h;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 pixel = get_gpixel_from_coord(magnitude, x, y);
            if (pixel == 255)
            { // Strong edge
                __edge_tracking_by_hysteresis(magnitude, x, y, width, height);
            }
        }
    }

    // Suppress remaining weak edges
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 pixel = get_gpixel_from_coord(magnitude, x, y);
            if (pixel == 128)
            {
                set_gpixel_from_coord(magnitude, x, y, 0); // Suppress weak edge
            }
        }
    }
}

void canny_edge_detection(SDL_Surface *surface, struct parameters *param)
{
    // STEP 1 : Grayscale image
    image_to_grayscale(surface, param);

    // STEP 2 : Gaussian blur
    int window_gaussian;
    // double *gaussian_mask = create_gaussian_mask_5x5(&window_gaussian);
    // convolve_surface(surface, gaussian_mask, window_gaussian);
    // free(gaussian_mask); // The caller is responsible for freeing the mask

    // STEP 3 : Sobel filter for gradient magnitude
    SDL_Surface *gradient_x = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0);
    SDL_Surface *gradient_y = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0);
    double *sobel_mask_x = get_sobel_mask_x();
    double *sobel_mask_y = get_sobel_mask_y();
    convolve_surface(gradient_x, sobel_mask_x, 3);
    convolve_surface(gradient_y, sobel_mask_y, 3);
    free(sobel_mask_y);
    free(sobel_mask_x);

    SDL_Surface *magnitude_gradient = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0);
    SDL_Surface *direction_gradient = SDL_CreateRGBSurface(0, surface->w, surface->h, 8, 0, 0, 0, 0);

    for (int j = 0; j < surface->h; j++)
    {
        for (int i = 0; i < surface->w; i++)
        {
            Uint8 pixel_x = get_gpixel_from_coord(gradient_x, i, j);
            Uint8 pixel_y = get_gpixel_from_coord(gradient_y, i, j);
            double magnitude = sqrt(pixel_x * pixel_x + pixel_y * pixel_y); // Euclidean distance formula
            double direction = atan2(pixel_y, pixel_x);                     // Gradient direction

            set_gpixel_from_coord(magnitude_gradient, i, j, (int)fmin(magnitude, 255));
            set_gpixel_from_coord(direction_gradient, i, j, (int)fmin((direction + PI) / (2 * PI) * 255, 255));
        }
    }

    // STEP 4 : Non-maximum suppression
    non_maximum_suppression(magnitude_gradient, direction_gradient);

    // STEP 5 : Double threshold for edge tracking
    double_threshold(magnitude_gradient);

    // STEP 6 : Edge tracking by hysteresis
    edge_tracking_by_hysteresis(magnitude_gradient);

    SDL_FreeSurface(gradient_x);
    SDL_FreeSurface(gradient_y);
    SDL_FreeSurface(magnitude_gradient);
    SDL_FreeSurface(direction_gradient);
}