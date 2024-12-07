#include "hough_transform.h"
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535897932 // 16 decimals

void hough_transform(const unsigned char *edges, int width, int height, int *accumulator, int acc_width, int acc_height)
{
    // Initialize the accumulator
    for (int i = 0; i < acc_width * acc_height; i++)
    {
        accumulator[i] = 0;
    }

    // Define the range of theta
    double theta_step = PI / acc_width;

    // Perform the Hough transform
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (edges[y * width + x] > 0)
            { // Edge pixel
                for (int theta_idx = 0; theta_idx < acc_width; theta_idx++)
                {
                    double theta = theta_idx * theta_step;
                    int rho = (int)(x * cos(theta) + y * sin(theta));
                    if (rho >= 0 && rho < acc_height)
                    {
                        accumulator[rho * acc_width + theta_idx]++;
                    }
                }
            }
        }
    }
}