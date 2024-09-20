#include <stdio.h>

int Length(char word[]) {
        // Takes a string, a list of characters
        // Returns the length of the word

        int i = 0;
        while (word[i] != 0) {
                i++;
        }
        return i;
}

int Solver(int row, int col, char[row][col], char word[]) {
        // Takes a matrice of characters and a word
        // Returns (for now) if the word can be in the matrice or not
        // --> The only thing checked for now is if the word can be included in terms of size

        int len = Length(word);
        int max = row > col ? row : col;
        // The maximum number of characters a word can have in this grid

        if (len > max) {
                return 1; // FALSE
        }
        else {
                return 0; // TRUE
        }
}

int main() {
        char word[] = "Hello";
        char mat[][2] =  { {'a', 'b'}, {'c', 'd'} };
        printf("can %s be in our matrice ?\n", word);
        int result = Solver(2, 2, mat, word);
        if (!result) printf("it can\n");
        else printf("it cant\n");
        result = Solver(2, 2, mat, "Hi");
        if (!result) printf("it can\n");
        else printf("it cant\n");
        char mat23[][3] =  { {'a', 'b', 'c'}, {'d', 'e', 'f'} };
        result = Solver(2, 3, mat23, word);
        if (!result) printf("it can\n");
        else printf("it cant\n");

        // printf("the length of the word is %i\n", Length("Hello"));
        return 0;
}
