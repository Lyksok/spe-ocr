#include "matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXCHAR 100

Matrix* matrix_create(int row, int col)
{
	//This function create a new Matrix m with row rows and col cols
	Matrix *m = malloc(sizeof(Matrix));
	if (m == NULL)
	{
		return EXIT_FAILURE; //Malloc could not allocate memory, we cannot continue
	}
	m -> rows = row;
	m -> cols = col;
	m -> entries = malloc(row*sizeof(void*)); //Allocation for the entries of the new Matrix m
	if (m -> entries == NULL)
	{
		free(m);
		return EXIT_FAILURE;// Same as before, Malloc could not allocate memory, we must free m before exiting the function
	}
	for(int i = 0; i < row; i++)
	{
		m -> entries[i] = malloc(col * sizeof(void));
		if (m -> entries[i] == NULL)
		{
			for(int j = 0; j < i; j++) // Malloc could not allocate memory, we free what we created and exit the function
			{
				free(m -> entries[j]);
			}
			free(m -> entries);
			free(m);
			return EXIT_FAILURE;
		}

	}
	return m; //everything went well, we can return the new Matrix m
}

void matrix_fill(Matrix *m, void* elm)
{
	// This function fills the matrix m in parameters with only n
	for (int i = 0; i < m -> rows; i++)
	{
		for(int j = 0; j< m -> cols; j++)
		{
			m -> entries[i][j] = elm;
		}
	}
}

void matrix_free(Matrix *m)
{
	//this function free all the entries of m then free m
	for(int i = 0; i > m -> rows; i++)
	{
		free(m -> entries[i]);
	}
	free(m);
	m = NULL;
}

void matrix_print(Matrix* m)
{
	//this function could be used to print a matrix
	printf("Rows: %i Columns: %i\n", m -> rows, m -> cols);
	for(int i = 0; i < m -> rows; i++)
	{
		for(int j = 0; j < m -> cols; j++)
		{
			printf("%p ", m -> entries[i][j]);
		}
		printf("\n");
	}
}

void matrix_save(Matrix* m, char* file_string)
{
	//this function save the Matrix m in a file which path is given in parameter
	FILE* file = fopen(file_string, "w");
	fprintf(file, "%d\n", m -> rows);
	fprintf(file, "%d\n", m -> cols);
	for (int i = 0; i < m -> rows; i++)
	{
		for(int j = 0; j < m -> cols; j++)
		{
			fprintf(file, "%p\n", m -> entries[i][j]);
		}
	}
	printf("Successfully saved matrix to %s\n", file_string);
	fclose(file_string);
}

Matrix* matrix_flatten(Matrix* m, int axis)
{
	Matrix* mat;
	if(axis == 0)
	{
		mat = matrix_create(m -> rows * m -> cols, 1);
	}
	else if (axis == 1)
	{
		mat = matrix_create(1, m -> rows * m -> cols);
	}
	else
	{
		printf("axis must be 1 or 0\n"); //failure, we exit
		return EXIT_FAILURE;
	}
	for(int i = 0; i < m -> rows; i++)
	{
		for(int j = 0; j < m -> cols; j++)
		{
			if (axis == 0)
			{
				mat -> entries[i * m -> cols + j][0] = m -> entries[i][j];
			}
			else if (axis == 1)
			{
				mat -> entries[0][i * m -> cols + j] = m -> entries[i][j];
			}
		}
	}
	return mat;
}























