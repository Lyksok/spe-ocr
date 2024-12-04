#include "gaussian.h"

// static double s = 1.0;
static int w = 5;

double *create_gaussian_mask_5x5(int *window)
{
    /*
    double* mask = calloc(w*w, sizeof(double));

    for(int y=-w/2; y<=w/2; y++)
    {
    int j=y+w/2;
    for(int x=-w/2; x<=w/2; x++)
    {
        int i=x+w/2;
        mask[j*w+i] = 1/(2*M_PI*s*s)*exp(-(x*x+y*y)/(2*s*s));
    }
    }

    *window=w;
    print_mask(mask);
    return mask;
    */
    double *mask = calloc(w * w, sizeof(double));
    double t_mask[] = {
        0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
        0.015019, 0.059913, 0.094907, 0.059913, 0.015019,
        0.023792, 0.094907, 0.150343, 0.094907, 0.023792,
        0.015019, 0.059913, 0.094907, 0.059913, 0.015019,
        0.003765, 0.015019, 0.023792, 0.015019, 0.003765};
    for (int i = 0; i < w * w; i++)
        mask[i] = t_mask[i];
    *window = w;
    // print_mask(mask);
    return mask;
}

double *create_gaussian_mask_3x3(int *window)
{
    double *mask = calloc(3 * 3, sizeof(double));
    double t_mask[] = {
        1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0,
        1.0 / 8.0, 1.0 / 4.0, 1.0 / 8.0,
        1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0};
    for (int i = 0; i < 3 * 3; i++)
        mask[i] = t_mask[i];
    *window = 3;
    // print_mask(mask);
    return mask;
}

void print_mask(double *mask)
{
    for (int j = 0; j < w; j++)
    {
        for (int i = 0; i < w; i++)
            printf("%lf ", mask[j * w + i]);
        printf("\n");
    }
}
