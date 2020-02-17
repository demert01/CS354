////////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row.c
// This File:        n_in_a_row.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Fall 2019
//
// Author:           Ye Ji Kim
// Email:            ykim@wisc.edu
// CS Login:         ykim
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>     

char *COMMA = ",";  

/*        
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}   


/* 
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int **board, int size) {
    int numX = 0; //number of X in the board
    int numO = 0; //number of O in the board
    int checkNumX[2][size]; //number of X in a row
    int checkNumO[2][size]; //number of O in a row
    int diagX[2]; //number of X on diagonal
    int diagO[2]; //number of O on diagonal
    int valid = 1; //if remain 1 then it is valid board

    int even = 0;
    if(size%2 == 0)
	even = 1;
    // printf("even: %d\n", even);

    //initialize diag arrays elements to 0
    *(diagX + 0) = 0;
    *(diagX + 1) = 0;
    *(diagO + 0) = 0;
    *(diagO + 1) = 0;

    //initialize checkNum arrays elements to 0
    for(int z=0; z<size; z++)
    {
	*(*(checkNumX + 0) + z) = 0;
	*(*(checkNumX + 1) + z) = 0;
        *(*(checkNumO + 0) + z) = 0;
	*(*(checkNumO + 1) + z) = 0;
    }

    for(int i=0; i<size; i++)
    {
	for(int j=0; j<size; j++)
	{
	    int elem = *(*(board+i)+j); // element of board
	    // check if all elements are 0 or 1 or 2
	    if(elem != 0 && elem != 1 && elem != 2)
	    { 
 	        valid = 0;
	    }
	    else if(1 == elem)
	    { // increase the number of X
	    	*(*(checkNumX + 0) + i) += 1;
		*(*(checkNumX + 1) + j) += 1;
	        numX++;
	    }
	    else if(2 == elem)
	    { // increas the number of O
	        *(*(checkNumO + 0) + i) += 1;
	        *(*(checkNumO + 1) + i) += 1;
	        numO++;
	    }
	}

	int diag1 = *(*(board + i) + i);
	int diag2 = *(*(board + i) + (size-1-i));
	// count the number of X and O on diagonal
	if(diag1 == 1)
	    *(diagX + 0) += 1;
	if(diag1 == 2)
	    *(diagO + 0) += 1;
	if(diag2 == 1)
	    *(diagX + 1) += 1;
	if(diag2 == 2)
	    *(diagO + 1) += 1;

    }

    // check if the board have at most 1 more X than O
    if((numX-numO) != 0 && (numX-numO) != 1){
	valid = 0;
    }  

    int winX = 0;
    int winO = 0;
    int winXrow = 0;
    int winXcol = 0;
    int winOrow = 0;
    int winOcol = 0;
    int diagXwin = 0;
    int diagOwin = 0;

    // count the number of winner
    if(*(diagX + 0) == size)
    {
        winX++;
	diagXwin++;
    }
    else if(*(diagX + 1) == size)
    {
        winX++;
	diagXwin++;
    }
    if(*(diagO + 0) == size)
    {
        winO++;
	diagOwin++;
    }
    else if(*(diagO + 1) == size)
    {
        winO++;
	diagOwin++;
    }

    // check if the board have two diagonal X or Os when size is even
    if(even)
    {
	if(diagXwin > 1)
	    valid = 0;
	if(diagOwin > 1)
	    valid = 0;
    }

    for(int k=0; k<size; k++)
    {
	if(*(*(checkNumX + 0) + k) == size)
	{
            winX++;
	    winXrow++;
	}
	else if(*(*(checkNumX + 1) + k) == size)
	{
            winX++;
	    winXcol++;
	}
	if(*(*(checkNumO + 0) + k) == size)
	{
            winO++;
	    winOrow++;
	}
        else if(*(*(checkNumO + 1) + k) == size)
	{
            winO++;
	    winOcol++;
	}
    }

    // check if the number of winner is valid
    if(winX * winO != 0){
        valid = 0;	    
    } else {
	// check if players keep played even when winner is decided
	// check if winner has more than 2 vertical or horizontal lines
   	if(winX && (numX == numO))
            valid = 0;
	else if(winXrow > 1 || winXcol > 1)
	    valid =0;
	if(winO && (numX > numO))
      	    valid = 0;
	else if(winOrow > 1 || winOcol > 1)
	    valid = 0;
    }
    
    //printf("num: %i\t, %i\n",numX, numO);
    //printf("win: %i\t, %i\n",winX, winO);
    
    return valid;
}     


/* 
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              

    //Check if number of command-line arguments is correct
    if(argc != 2)
    {
	printf("Usage: ./n_in_a_row <input_filename>\n");
    	exit(1);
    }
    
    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Cannot open file for reading\n");
        exit(1);
    }

    //Declare local variables.
    int size;

    //Called get_dimensions to retrieve the board size
    get_dimensions(fp, &size);
	
    //Dynamically allocate a 2D array of dimensions retrieved above.
    int **board;
    board = malloc(sizeof(int*)*size);
    for(int i = 0; i < size; i++) 
    {
	*(board + i) = malloc(sizeof(int)*size);
    }

    //Read the file line by line.
    //Tokenize each line wrt comma to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++)
    {
        if (getline(&line, &len, fp) == -1)
       	{
            printf("Error while reading the file\n");
            exit(1);
        }

        token = strtok(line, COMMA);
        for (int j = 0; j < size; j++)
       	{
            //initialize 2D array.
            *(*(board + i) + j) = atoi(token);
            token = strtok(NULL, COMMA);
        }
    }
    
    //Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    if(n_in_a_row(board, size) == 1)
    { 
	printf("valid\n");
    } else {
	printf("invalid\n");
    }


    //Free all dynamically allocated memory.
    for(int i = 0; i < size; i++)
    {
	free(*(board+i));
    }
    free(board);
    board = NULL;

    //Close the file.
    if (fclose(fp) != 0)
    {
        printf("Error while closing the file\n");
        exit(1);
    } 

    return 0;       
}       

