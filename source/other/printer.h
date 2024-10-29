#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>
void print_list_int(int** list, size_t size);
void print_list_double(double** list, size_t size);
void print_matrix(size_t row, size_t col, char mat[row][col]);

#endif	/* PRINTER_H */
