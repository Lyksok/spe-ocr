#ifndef HOUGH_TRANSFORM_H
#define HOUGH_TRANSFORM_H

void hough_transform(const unsigned char *edges, int width, int height,
                     int *accumulator, int acc_width, int acc_height);

#endif // HOUGH_TRANSFORM_H