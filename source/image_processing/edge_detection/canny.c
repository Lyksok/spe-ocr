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

void non_maximum_suppression(SDL_Surface *surface, Uint8 **magnitude, Uint8 **direction)
{
    int width = surface->w;
    int height = surface->h;

    // Allocate space for suppressed magnitude
    Uint8 *suppressed = calloc(width * height, sizeof(Uint8));

    for (int y = 1; y < height - 1; y++) // Exclude borders
    {
        for (int x = 1; x < width - 1; x++)
        {
            Uint8 mag = (*magnitude)[y * width + x];
            double dir = ((*direction)[y * width + x] * 2.0 * PI / 255.0) - PI;

            Uint8 neighbor1 = 0;
            Uint8 neighbor2 = 0;

            // Determine the neighboring pixels based on gradient direction
            if ((-PI / 8 <= dir && dir < PI / 8) || (7 * PI / 8 <= dir || dir < -7 * PI / 8))
            {
                neighbor1 = (*magnitude)[y * width + (x + 1)]; // Right
                neighbor2 = (*magnitude)[y * width + (x - 1)]; // Left
            }
            else if ((PI / 8 <= dir && dir < 3 * PI / 8) || (-7 * PI / 8 <= dir && dir < -5 * PI / 8))
            {
                neighbor1 = (*magnitude)[(y - 1) * width + (x + 1)]; // Top-right
                neighbor2 = (*magnitude)[(y + 1) * width + (x - 1)]; // Bottom-left
            }
            else if ((3 * PI / 8 <= dir && dir < 5 * PI / 8) || (-5 * PI / 8 <= dir && dir < -3 * PI / 8))
            {
                neighbor1 = (*magnitude)[(y - 1) * width + x]; // Top
                neighbor2 = (*magnitude)[(y + 1) * width + x]; // Bottom
            }
            else if ((5 * PI / 8 <= dir && dir < 7 * PI / 8) || (-3 * PI / 8 <= dir && dir < -PI / 8))
            {
                neighbor1 = (*magnitude)[(y - 1) * width + (x - 1)]; // Top-left
                neighbor2 = (*magnitude)[(y + 1) * width + (x + 1)]; // Bottom-right
            }

            // Keep the pixel if it is greater than or equal to both neighbors
            if (mag >= neighbor1 && mag >= neighbor2)
            {
                suppressed[y * width + x] = mag;
            }
            else
            {
                suppressed[y * width + x] = 0;
            }
        }
    }

    // Replace the magnitude with the suppressed version
    free(*magnitude);
    *magnitude = suppressed;
}

void double_threshold(SDL_Surface *surface, Uint8 **magnitude)
{
    int width = surface->w;
    int height = surface->h;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 pixel = (*magnitude)[y * surface->w + x];
            if (canny_double_threshold_low > pixel) // non-edges : black, to delete
            {
                (*magnitude)[y * surface->w + x] = 0;
            }
            else if (pixel > canny_double_threshold_high) // strong edge : white, to keep
            {
                (*magnitude)[y * surface->w + x] = 255;
            }
            else // weak edge : in between, determined in step 6 : if connected to strong edge => strong edge else non-edge
                (*magnitude)[y * surface->w + x] = 128;
        }
    }
}

int is_within_bounds(int x, int y, int width, int height)
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

void __edge_tracking_by_hysteresis(SDL_Surface *surface, Uint8 **magnitude, int x, int y, int width, int height)
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
                Uint8 neighbor_pixel = (*magnitude)[ny * surface->w + nx];
                if (neighbor_pixel == 128)
                {
                    (*magnitude)[ny * surface->w + nx] = 255;                                 // Becomes to strong edge!
                    __edge_tracking_by_hysteresis(surface, magnitude, nx, ny, width, height); // Recursive call
                }
            }
        }
    }
}
void edge_tracking_by_hysteresis(SDL_Surface *surface, Uint8 **magnitude)
{
    int width = surface->w;
    int height = surface->h;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 pixel = (*magnitude)[y * surface->w + x];
            if (pixel == 255)
            { // Strong edge
                __edge_tracking_by_hysteresis(surface, magnitude, x, y, width, height);
            }
        }
    }

    // Suppress remaining weak edges
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Uint8 pixel = (*magnitude)[y * surface->w + x];
            if (pixel == 128)
            {
                (*magnitude)[y * surface->w + x] = 0; // Suppress weak edge
            }
        }
    }
}

Uint8 *apply_mask(SDL_Surface *surface, double *mask, int w)
{
    int len = surface->w * surface->h;

    Uint8 *res = calloc(len, sizeof(Uint8));

    for (int j = 0; j < surface->h; j++)
    {
        for (int i = 0; i < surface->w; i++)
        {
            Uint8 pixel = get_gpixel_from_coord(surface, i, j);
            double sum = 0;
            for (int y = -w / 2; y <= w / 2; y++)
            {
                for (int x = -w / 2; x <= w / 2; x++)
                {
                    if (i + x < 0 || i + x >= surface->w || j + y < 0 || j + y >= surface->h)
                    {
                        sum += ((double)(255 * mask[(w / 2 + y) * w + x + w / 2])) / 9.0;
                    }
                    else
                    {
                        Uint8 local_pixel = get_gpixel_from_coord(surface, i + x, j + y);
                        sum += ((double)(local_pixel * mask[(w / 2 + y) * w + x + w / 2])) / 9.0;
                    }
                }
            }
            res[j * surface->w + i] = (Uint8)sum;
        }
    }
    return res;
}
void apply_sobel_filter(SDL_Surface *surface, Uint8 **magnitude_gradient, Uint8 **direction_gradient)
{
    int len = surface->w * surface->h;

    // Allocate memory for gradients
    Uint8 *gradient_x = calloc(len, sizeof(Uint8));
    Uint8 *gradient_y = calloc(len, sizeof(Uint8));

    // Get Sobel masks
    double *sobel_mask_x = get_sobel_mask_x();
    double *sobel_mask_y = get_sobel_mask_y();

    // Apply masks
    Uint8 *x_gradient = apply_mask(surface, sobel_mask_x, 3);
    Uint8 *y_gradient = apply_mask(surface, sobel_mask_y, 3);
    free(sobel_mask_y);
    free(sobel_mask_x);

    // Compute magnitude and direction
    for (int j = 0; j < surface->h; j++)
    {
        for (int i = 0; i < surface->w; i++)
        {
            Uint8 pixel_x = x_gradient[j * surface->w + i];
            Uint8 pixel_y = y_gradient[j * surface->w + i];

            // Calculate magnitude
            double magnitude = sqrt(pixel_x * pixel_x + pixel_y * pixel_y);

            // Calculate direction and scale to [0, 255]
            double direction = atan2(pixel_y, pixel_x);
            direction = (direction + PI) / (2 * PI) * 255;

            // Clamp and store results
            (*magnitude_gradient)[j * surface->w + i] = (Uint8)fmin(magnitude, 255);
            (*direction_gradient)[j * surface->w + i] = (Uint8)fmin(direction, 255);
        }
    }

    // Free intermediate gradients
    free(x_gradient);
    free(y_gradient);
    free(gradient_x);
    free(gradient_y);
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