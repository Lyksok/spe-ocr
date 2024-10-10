#include <stdio.h>
#include "solver.h"

int Length(char word[]) {
        // Takes a string, a list of characters
        // Returns the length of the word

        int i = 0;
        while (word[i] != 0) {
                i++;
        }
        return i;
}

int Max(int a, int b) {
	// returns the max between a and b
	return a > b ? a : b;
}

int Min(int a, int b) {
	//returns the min between a and b
	return a < b ? a : b;
}

int Search(int row, int col, char mat[row][col], char word[], int r, int c, int len) {
	// we enter here when we found a match for the first letter
	
	// will give us the direction in which to search
	int x[] = { -1, -1, -1, 0, 0, 1, 1, 1 };	// rows
	int y[] = { -1, 0, 1, -1, 1, -1, 0, 1 };	// columns
	
	int dir = 0;
	int i;		// current index in our word
	int ri;		// current row in our direction
	int ci;		// current column in our direction
	int found = 0;	// set to false at the beginning
	int noStop;	// use to check if we encounter a stopping case

	while (dir < 8 && !found) {
		ri = r + x[dir];
		ci = c + y[dir];

		i = 1;		// we already checked the first character before entering the function
		noStop = 1;	// set to true here 
		
		while (i < len && noStop) {
			if (ri >= row || ri < 0 || ci >= col || ci < 0) {
				// out of bounds, set to FALSE
				noStop = 0;
			}
			else if (mat[ri][ci] != word[i]) {
				// no match, set to FALSE
				noStop = 0;
			}
			else {
				// we have a match, we continue in our directions
				i++;
				ri += x[dir];
				ci += y[dir];
			}
		}

		found = noStop;
		dir++;
	}


	return found;

}

int Solver(int row, int col, char mat[row][col], char word[]) {
        // Takes a matrice of characters and a word
	// The function is case sensitive for now :
	// it does not check for variance in capital and lower letters !
        // Returns (for now) if the word can be in the matrice or not
        // --> The only thing checked for now is if the word can be included in terms of size

        int len = Length(word);
	int max = Max(row, col);
        // int max = row > col ? row : col;
        // The maximum number of characters a word can have in this grid

        if (len > max) {
                return 0; // we have an error, convention is to return 1 
			  // 1 == TRUE
			  // BUT WE DONT CARE, I'm just doing a true/false
			  // so let's return 0 (false)
        }
        else {
		// it can be in term of size
		// return 0 is used by convention to end a function normally 
		// BUT here we don't care about convention as it is a true/false 
		// 1 == TRUE 
		// 0 == FALSE 


		int r = 0;	// iterate over rows
		int c = 0;	// iterate over columns

		char found = 0;	// set to FALSE
				// used to check if found or not

		while (r < row && !found) {
			while ( c < col && !found) {
				if (mat[r][c] == word[0]) {
					// call function ?
					// have to check the rest of the matrice
					found = Search(row, col, mat, word, r, c, len);
					// we want to get TRUE if the function terminated normally
					// the convention is to return 0 if terminated without problem
					// but 0 means FALSE, so we !(result) in order to get the actual result
				}
				c++;
			}
			r++;
		}

		return found;
        }
}

