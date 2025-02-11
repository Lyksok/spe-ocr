#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

// 1 == TRUE
// 0 == FALSE

/*
 * word : a string, list of caracters
 * Returns the length of the word
 */
int Length(char word[])
{
  int i = 0;
  while (word[i] != 0)
    i++;
  return i;
}

/*
 * Returns the max between a and b
 */
int Max(int a, int b) { return a > b ? a : b; }

/*
 * Returns the min between a and b
 */
int Min(int a, int b) { return a < b ? a : b; }

/*
 * word : a string
 * Changes in place the word to upper letters
 * Boolean to check if characters other than letter was found
 */
int NotJustLetters(char word[])
{
  size_t i = 0;
  while (word[i] != 0)
  {
    char c = word[i];
    if (c >= 'a' && c <= 'z')
      word[i] = c - 32;
    else if (c >= 'A' && c <= 'Z')
    {
      // pass
    }
    else
    {
      printf("The word should only have letters\n");
      return 1;
    }
    i++;
  }
  return 0;
}

/*
 * word : a string, assumed filled with only letters
 * Changes in place the word to upper letters
 */
void ToUpper(char word[])
{
  size_t i = 0;
  while (word[i] != 0)
  {
    if (word[i] >= 'a')
      word[i] = word[i] - 32;
    i++;
  }
}

/*
 * We only enter this function when we found a match for the first letter
 * mat : a matrice of characters
 * word : the word to search for
 * n : the length of the word
 * er : a pointer to the row of the last letter
 * ec : a pointer to the column of the last letter
 * Returns True or False if the word is found
 */
int Search(int row, int col, char **mat, char *word, int r, int c, int n,
           int *er, int *ec)
{
  /*
   * we start the ends from the match
   * in case the word is made of only one letter
   */
  *er = r;
  *ec = c;

  /*
   * will give us the direction in which to search
   * x is for the rows
   * y is for the columns
   * dir is the index over which we will iterate
   */
  int x[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int y[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int dir = 0;

  /*
   * i : current index in the word
   * ri : current row in our direction
   * ci : current column in our direction
   */
  int i;
  int ri;
  int ci;

  /*
   * found : boolean (false at the beginning)
   * noStop : boolean checking if we encounter a stopping case
   */
  int found = 0;
  int noStop;

  while (dir < 8 && !found)
  {
    // we update the direction
    ri = r + x[dir];
    ci = c + y[dir];

    // we already found the first character
    i = 1;
    // set to True
    noStop = 1;

    while (i < n && noStop)
    {
      *er = ri;
      *ec = ci;
      if (ri >= row || ri < 0 || ci >= col || ci < 0)
      {
        // out of bounds :
        // set to FALSE
        noStop = 0;
      }
      else if (mat[ri][ci] != word[i])
      {
        // no match :
        // set to FALSE
        noStop = 0;
      }
      else
      {
        // we have a match :
        // we continue in our directions
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

/*
 * mat : a matrice of characters
 * word : the word to search for
 * sr and sc : starting row and column of the word
 * er and ec : ending row and column of the word
 * Returns True or False if the word is found
 */
int Solver(int row, int col, char **mat, char *word, int len, int *sr, int *sc,
           int *er, int *ec)
{
  /*
   * len : the length of the word
   * max : the max between the numbers of rows and columns
   */
  // int len = Length(word);
  int max = Max(row, col);

  // if the word is bigger than what is possible :
  // return False
  if (len > max)
  {
    printf("Not Found\n");
    return 0;
  }
  else
  {
    /*
     * r : to iterate over rows
     * c : to iterate over columns
     * found : boolean, have we found it, set to False
     * sr : last starting row if match
     * sc : last starting column if match
     * er : ending row if it exists
     * ec : ending column if it exists
     */
    int r = 0;
    int c = 0;
    char found = 0;
    *sr = 0;
    *sc = 0;
    *er = 0;
    *ec = 0;

    while (r < row && !found)
    {
      c = 0;
      while (c < col && !found)
      {
        if (mat[r][c] == word[0])
        {
          // the first letter of the word match
          // we check for the rest
          *sr = r;
          *sc = c;
          found = Search(row, col, mat, word, *sr, *sc, len, er, ec);
        }
        c++;
      }
      r++;
    }

    if (found)
      printf("(%i,%i)(%i,%i)\n", *sc, *sr, *ec, *er);
    else
      printf("Not Found\n");

    return found;
  }
}
