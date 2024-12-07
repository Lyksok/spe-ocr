#include "edge_detection.h"

double calculate_rotation_angle(int *accumulator, int acc_width, int acc_height, double theta_step)
{
    int max_votes = 0;
    int best_theta_idx = 0;

    // Find the angle with the most votes
    for (int theta_idx = 0; theta_idx < acc_width; theta_idx++)
    {
        for (int rho = 0; rho < acc_height; rho++)
        {
            int votes = accumulator[rho * acc_width + theta_idx];
            if (votes > max_votes)
            {
                max_votes = votes;
                best_theta_idx = theta_idx;
            }
        }
    }

    // Calculate the angle in degrees
    double best_theta = best_theta_idx * theta_step;
    double angle = best_theta * 180.0 / PI;
    return angle;
}

double auto_detect_rotation_angle(SDL_Surface *surface, struct parameters *param)
{
    // Step 1: Apply Canny edge detection
    canny_edge_detection(surface, param);

    // Step 2: Prepare for Hough transform
    int width = surface->w;
    int height = surface->h;
    int acc_width = 180;                                         // Number of angle steps
    int acc_height = (int)sqrt(width * width + height * height); // Max rho value
    int *accumulator = calloc(acc_width * acc_height, sizeof(int));
    double theta_step = PI / acc_width;

    // Step 3: Apply Hough transform
    Uint8 *edges = surface->pixels;
    hough_transform(edges, width, height, accumulator, acc_width, acc_height);

    // Step 4: Calculate the rotation angle
    double angle = calculate_rotation_angle(accumulator, acc_width, acc_height, theta_step);

    // Clean up
    free(accumulator);

    return angle;
}