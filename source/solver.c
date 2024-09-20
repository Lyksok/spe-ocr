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

int Max(int a, int b) {
	// returns the max between a and b
	a > b ? a : b;
}

int Min(int a, int b) {
	//returns the min between a and b
	a < b ? a : b;
}

int Search(int row, int col, char mat[row][col], char word[], int r, int c, int len) {
	// we enter here when we found a match for the first letter
	
	// will give us the direction in which to search
	int x[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int y[] = { -1, 0, 1, -1, 1, -1, 0, 1 }; 
	
	int dir = 0;
	int i;
	int ri;		// current row in our direction
	int ci;		// current column in our direction
	int found = 1;
	int noStop = 0;	// use to check if we encounter a stopping case

	while (dir < 8 && !found) {
		ri = r + x[dir];
		ci = c + y[dir];

		i = 1;	// we already checked the first character before entering the function
		
		while (i < len && noStop) {
			if (ri >= row || ri < 0 || ci >= col || ci < 0) {
				// out of bounds
				noStop = 1;
			}
			else if (mat[ri][ci] != word[i]) {
				// no match
				noStop = 1;
			}
			else {
				// we have a match, we continue in our directions
				i++;
				ri += x[dir];
				ci += y[dir];
			}
		}
		if (noStop) {
			// we found our word
			found = 0;
		}
		else {
			dir++;
		}
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
                return 1; // FALSE
        }
        else {
		// it can be in term of size
                // return 0; // TRUE
		int r = 0;
		int c = 0;
		int i = 0;

		char found = 1;

		while (r < row && !found) {
			while ( c < col && !found) {
				if (mat[r][c]==word[i]) {
					// call function ?
					// have to check the rest of the matrice
					found = Search(row, col, mat, word, r, c, len);
				}
				c++;
			}
			r++;
		}

		return found;
        }
}

int main() {
        //char word[] = "Hello";
	//char mat[][2] =  { {'a', 'b'}, {'c', 'd'} };
	//printf("can %s be in our matrice ?\n", word);
	//int result = Solver(2, 2, mat, word);
	//if (!result) printf("it can\n");
	//else printf("it cant\n");
	//result = Solver(2, 2, mat, "Hi");
	//if (!result) printf("it can\n");
	//else printf("it cant\n");
	//char mat23[][3] =  { {'a', 'b', 'c'}, {'d', 'e', 'f'} };
	//result = Solver(2, 3, mat23, word);
	//if (!result) printf("it can\n");
	//else printf("it cant\n");

	char word[] = "Hi";
	char mat[][2] =  { {'a', 'b'}, {'c', 'd'} };
	int result = Solver(2, 2, mat, word);

	printf("can %s be in our matrice ?\n", word);
	if (!result) printf("it can\n");
        else printf("it cant\n");

	char mat2[][2] = { {'H', 'i'}, {'c', 'd'} };
	result = Solver(2, 2, mat2, word);
	if (!result) printf("it can\n");
        else printf("it cant\n");


        // printf("the length of the word is %i\n", Length("Hello"));
        return 0;
}
