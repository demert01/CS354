///////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dclash.c
// This File:        cache2Dclash.c
// Other Files:      cache1D.c cache2Drows.c cache2Dcols.c cache2Dclash.c
// Semester:         CS 354 Fall 2019
//
// Author:           Ye Ji Kim
// Email:            ykim762@wisc.edu
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
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <string.h>
 
int arr2D[128][8]; // global variable so that it will be in data segment
 
/*
 * This is the main function to iterate over the entire array and set
 * the value of each element in the array to its index
 */
int main() {

   for(int i=0; i<100; i++) {
	for(int row=0; row<128; row+=64) {
	    for(int col=0; col<8; col++) {
		arr2D[row][col] = i + row + col;
	    }
	}
   }
   return 0;
}

