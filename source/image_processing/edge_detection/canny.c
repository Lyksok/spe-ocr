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
    // Get the width and height of the image
    int width = surface->w;
    int height = surface->h;
    Uint8 *suppressed = calloc(width * height, sizeof(Uint8)); // Create a dsdl surface to store the suppressed image

    for (int y = 1; y < height - 1; y++)
    { // Loop through each row of the image, excluding the borders
        for (int x = 1; x < width - 1; x++)
        {
            // Loop through each column of the image, excluding the borders
            Uint8 mag = (*magnitude)[y * surface->w + x];                            // Get the gradient magnitude for the current pixel
            double dir = ((*direction)[y * surface->w + x] * 2.0 * PI / 255.0) - PI; // Get the gradient direction for the current pixel and convert it to radi

            /** By initializing neighbour1 and neighbour2 to 255, we ensure that if the gradient direction does not match any of the specified ranges, the current pixel will be suppressed because its magnitude will not be greater than or equal to 255.*/
            Uint8 neighbour1 = 255;
            Uint8 neighbour2 = 255;

            // Determine the neighboring pixels to compare based on the gradient direction
            if ((0 <= dir && dir < PI / 8) || (7 * PI / 8 <= dir && dir <= PI))
            {
                neighbour1 = (*magnitude)[(y + 1) * surface->w + x]; // Get the pixel value to the right
                neighbour2 = (*magnitude)[(y - 1) * surface->w + x]; // Get the pixel value to the left
            }
            else if (PI / 8 <= dir && dir < 3 * PI / 8)
            {
                neighbour1 = (*magnitude)[(y - 1) * surface->w + x + 1]; // Get the pixel value to the top-right
                neighbour2 = (*magnitude)[(y - 1) * surface->w + x - 1]; // Get the pixel value to the bottom-left
            }
            else if (3 * PI / 8 <= dir && dir < 5 * PI / 8)
            {
                neighbour1 = (*magnitude)[y * surface->w + x + 1]; // Get the pixel value to the top
                neighbour2 = (*magnitude)[y * surface->w + x - 1]; // Get the pixel value to the bottom
            }
            else if (5 * PI / 8 <= dir && dir < 7 * PI / 8)
            {
                neighbour1 = (*magnitude)[(y - 1) * surface->w + x - 1]; // Get the pixel value to the top-left
                neighbour2 = (*magnitude)[(y + 1) * surface->w + x + 1]; // Get the pixel value to the bottom-right
            }

            // Suppress the non-maximum pixels
            if (mag >= neighbour1 && mag >= neighbour2)
            {
                suppressed[y * surface->w] = mag; // Keep the pixel if = current max
            }
            else
            {
                suppressed[y * surface->w + x] = 0; // else suppress cur pixel
            }
        }
    }
    // Copy suppressed surface => original magnitude surface

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

void canny_edge_detection(SDL_Surface *surface, struct parameters *param)
{
    // STEP 1 : Grayscale image
    image_to_grayscale(surface, param);

    // STEP 2 : Gaussian blur
    //int window_gaussian;
    // double *gaussian_mask = create_gaussian_mask_5x5(&window_gaussian);
    // convolve_surface(surface, gaussian_mask, window_gaussian);
    // free(gaussian_mask); // The caller is responsible for freeing the mask

    // STEP 3 : Sobel filter for gradient magnitude
    int len = surface->w * surface->h;
    Uint8 *gradient_x = calloc(len, sizeof(Uint8));
    Uint8 *gradient_y = calloc(len, sizeof(Uint8));

    double *sobel_mask_x = get_sobel_mask_x();
    double *sobel_mask_y = get_sobel_mask_y();

    Uint8 *magnitude_gradient = calloc(len, sizeof(Uint8));
    Uint8 *direction_gradient = calloc(len, sizeof(Uint8));

    Uint8 *x_gradient = apply_mask(surface, sobel_mask_x, 3);
    Uint8 *y_gradient = apply_mask(surface, sobel_mask_y, 3);
    free(sobel_mask_y);
    free(sobel_mask_x);

    for (int j = 0; j < surface->h; j++)
    {
        for (int i = 0; i < surface->w; i++)
        {
            Uint8 pixel_x = gradient_x[j * surface->w + i];
            Uint8 pixel_y = gradient_y[j * surface->w + i];
            double magnitude = sqrt(pixel_x * pixel_x + pixel_y * pixel_y); // Euclidean distance formula
            double direction = atan2(pixel_y, pixel_x);                     // Gradient direction

            magnitude_gradient[j * surface->w + i] = (Uint8)fmin(magnitude, 255);
            direction_gradient[j * surface->w + i] = (Uint8)fmin((direction + PI) / (2 * PI) * 255, 255);
        }
    }

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
    free(x_gradient);
    free(y_gradient);
}
