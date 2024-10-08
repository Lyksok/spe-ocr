#include <stdio.h>
#include "solver.h"

void canIt(int row, int col, char mat[row][col], char * word) {
        printf("Can %s be in our matrice : ", word);
        int result = !Solver(row, col, mat, word);
        if (result) printf("Yes !\n");
        else printf("Nope\n");
}

void abcd(char * word) {
	char mat[][2] =  { {'a', 'b'}, {'c', 'd'} };
	canIt(2, 2, mat, word);
}

void abcdef(char * word) {
	char mat[][3] =  { {'a', 'b', 'c'}, {'d', 'e', 'f'} };
	canIt(2, 3, mat, word);
}

void Hiv0(char * word) {
	char mat[][1] = { {'H'}, {'i'} };
	canIt(2, 1, mat, word);
}

void Hiv1(char * word) {
	char mat[][2] =  { {'H', 'i'}, {'c', 'd'} };
	canIt(2, 2, mat, word);
}

void Hiv2(char * word) {
        char mat[][2] =  { {'H', 'o'}, {'i', 'd'} };
	canIt(2, 2, mat, word);
}

void Hiv3(char * word) {
        char mat[][2] =  { {'o', 'i'}, {'H', 'd'} };
	canIt(2, 2, mat, word);
}

void Hiv4(char * word) {
        char mat[][2] =  { {'H', 'o'}, {'c', 'i'} };
	canIt(2, 2, mat, word);
}

// int main(int argc, char *argv[]) 
int main () {	
	abcd("nothing");
	Hiv0("Hi");
	Hiv1("Hi");
	Hiv0("Hello");
        return 0;
}
