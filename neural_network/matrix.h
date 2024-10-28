#pragma once

typedef struct {
	void** entries;
	int rows;
	int cols;
} Matrix;

Matrix* matrix_create(int row, int col);
void matrix_fill(Matrix *m, void* elm);
void matrix_free(Matrix *m);
void matrix_print(Matrix *m);
void matrix_save(Matrix* m, char* file_string);
Matrix* matrix_flatten(Matrix* m, int axis);
