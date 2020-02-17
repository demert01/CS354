///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
///////////////////////////////////////////////////////////////////////////////
// Main File:        hill_climb.c
// This File:        hill_climb.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Fall 2019
//
// Author:           Ye Ji Kim
// Email:            ykim762@wisc.edu
// CS Login:         ykim
//
/////////////////////////// OTHER SOURCES OF HELP /////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>   
#include <stdlib.h>     
#include <string.h>  

/*
 * USE THIS:
 * Structure representing the Matrix
 * numRows: number of rows
 * numCols: number of columns
 * matrix: 2D heap allocated array of integers
 */
typedef struct {    
    int numRows;
    int numCols;
    int **matrix;
} Matrix;

/* 
 * Get the dimensions of the matrix that are on
 * the first line of the input file.
 * 
 * infp: file pointer for input file
 * nRows: pointer to number of rows in the matrix   
 * nCols: pointer to number of columns in the matrix
 */
void get_dimensions(FILE *infp, int *nRows, int *nCols) {         
    // Read the dimensions on the first line from infp
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, infp) == -1)
    {
        printf("Error in reading the file\n");
        exit(1);
    }

    char *token = NULL;

    // get first token    
    token = strtok(line, ",");
    *nRows = atoi(token);

    // get second token
    token = strtok(NULL, ",");
    *nCols = atoi(token);
//  printf("row: %d, col: %d \n", *nRows, *nCols);
}            

/* 
 * Continually find and print the largest neighbor
 * that is also larger than the current position
 * until nothing larger is found.       
 * 
 * outfp: file pointer for output file  
 * mstruct: pointer to the Matrix structure
 */
void hill_climb(FILE *outfp, Matrix *mstruct) {  
    // initialize the board with row and col
    int **board = mstruct -> matrix;
    int rows = mstruct -> numRows;
    int cols = mstruct -> numCols;

    // write the initial number to file
    int init = *(*(board+0)+0);
    fprintf(outfp,"%d ",init);

    int currow = 0; // store current row 
    int curcol = 0; // stroe current col
    int cur = init;

    int boolean = 1; // for while loop

    // while loop until can't find anything larger
    while(boolean){
	int maxrow = currow;
        int maxcol = curcol;
	int max = cur;

	// Fine the largest neighbor that is also larger than the current pos
	for(int i = currow-1; i < currow+2; i++) {
	    for(int j = curcol-1; j < curcol+2; j++) { 
		// if i and j are on the board
		if(i>=0 && j>=0 && i<rows && j<cols) {
		    // if current position changed
		    if(i != currow || j != curcol){
			// if max is less than the curent number
			if(max < (*(*(board + i) + j)) ) {
			    // store that row and col, update max
			    maxrow = i;
			    maxcol = j;
			    max = (*(*(board+i)+j));
			}
		    }
		}
	    }
	}

	// Can't find anything larger
	if(maxrow == currow && maxcol == curcol && max == cur){
	    fprintf(outfp, "\n");
	    boolean = 0; // break the loop
	} else {
	    // Move to that position
	    currow = maxrow;
	    curcol = maxcol;
	    cur = max;
	    fprintf(outfp, "%d ", cur); // Write the number to outfp
	}
    }    
}    

/* 
 * This program reads an m by n matrix from the input file
 * and outputs to a file the path determined from the
 * hill climb algorithm.    
 *
 * argc: CLA count
 * argv: CLA values
 */
int main(int argc, char *argv[]) {
    // Check if number of command-line arguments is correct
    if(argc != 3)
    {
	printf("Usage: ./hill_climb <input_filename> <output_filename>\n");
	exit(1);
    }

    // Open the input file and check if it opened successfully
    FILE *ifp = fopen(*(argv + 1), "r");
    if(ifp == NULL) 
    {
        printf("Cannot open file for reading.\n");
        exit(1);
    }

    // Declare local variables including the Matrix structure
    int rows = 0;
    int cols = 0;

    // Call the function get_dimensions
    get_dimensions(ifp, &rows, &cols);
    
    // Dynamically allocate a 2D array in the Matrix structure
    int **arr;
    arr = malloc(sizeof(int*)*rows);
    for(int k=0; k<rows; k++)
    {
	*(arr + k) = malloc(sizeof(int*)*cols);		
    }

    // Read the file line by line base on the number of rows
    // Tokenize each line wrt comma to store the values in the matrix
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
   
    for(int i=0; i<rows; i++)
    {
	if(getline(&line, &len, ifp) == -1)
	{
	    printf("Error while reading the file \n");
	    exit(1);
	}

	token = strtok(line, ",");
	
	for(int j=0; j<cols; j++)
	{
	    *(*(arr + i) + j) = atoi(token);
	    token = strtok(NULL, ",");
	}
    }

    // Open the output file  
    FILE *ofp = fopen(*(argv + 2), "w");
    if(ofp == NULL)
    {
	printf("Cannot open file for writing.\n");
	fclose(ifp);
	exit(1);
    }

    // Initialize the structure
    Matrix myMat; 
    myMat.numRows = rows;
    myMat.numCols = cols;
    myMat.matrix = arr;

    // Call the function hill_climb
    hill_climb(ofp, &myMat); 

    // Free the dynamically allocated memory
    for(int z=0; z<rows; z++)
    {
	free(*(arr + z));
    }
    free(arr);
    arr = NULL;

    // Close the input file
    if(fclose(ifp) != 0)
    {
	printf("Error while closing the input file\n");
	exit(1);
    }

    // Close the output file
    if(fclose(ofp) != 0)
    {
        printf("Error while closing the output file\n");
	exit(1);
    }

    return 0; 
}   
