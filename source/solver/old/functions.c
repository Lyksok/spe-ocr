// ----- PREVIOUS MAIN -----
int main(int argc, char* argv[])
{
        if (argc != 3)
        {
                int a = argc - 1;
                printf("Expected 2 arguments, but got %i\n", a);
                return 1;
        }

        /*
         * row : pointer to the number of rows
         * col : pointer to the number of columns
         * grid : our matrix we want to search in
         */
        int *row = calloc(1, sizeof(int));
        int *col = calloc(1, sizeof(int));
        if (row == NULL || col == NULL)
        {
                printf("Memory allocation failed (row/col)\n");
                return 2;
        }

        char **grid = ReadFile(argv[1], row, col);
        if (grid == NULL)
        {
                printf("Could not create the grid\n");
                return 3;
        }

        /*
         * This function checks for none letter characters
         * It also changes the word in place to uppercase
         */
        if (NotJustLetters(argv[2]))
        {
                FreeMat(grid, *row);
                free(row);
                free(col);
                return 4;
        }

        /* FOR TESTING PURPOSE
        PrintMat(*row, *col, grid);
        PrintWord(argv[2]);
        */

        Solver(*row, *col, grid, argv[2]);

        FreeMat(grid, *row);
        free(row);
        free(col);
        return 0;
}
