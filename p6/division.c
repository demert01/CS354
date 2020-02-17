////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      intdate.c sendsig.c division.c
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
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//global variable
int count = 0; //count the number of successful division

/**
 * SIGFPE handler
 * when a division by 0 occur
 *
 * sig: signal number
 */
void sigfpe_handler(int sig) {
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", count);
    printf("The program will be terminated.\n");
    exit(0);
}

/**
 * SIGINT handler
 * when Ctrl+c is typed
 *
 * sig: signal number
 */
void sigint_handler(int sig) {
    printf("\nTotal number of operations successfully completed: %d\n", count);
    printf("The program will be terminated.\n");
    exit(0);
}

/**
 * The main function of program
 * ask user for input and set handlers
 *
 * argc - number of arguments
 * argv - array of arguments
 */
int main(int args, char* arg[]){

    struct sigaction sig_fpe, sig_int;

    // SIGFPE setting
    if(memset(&sig_fpe, 0, sizeof(sig_fpe)) != &sig_fpe) {
        printf("Error from setting memory\n");
        exit(1);
    }
    // setting SIGFPE
    sig_fpe.sa_handler = sigfpe_handler;
    sig_fpe.sa_flags = 0; // restart system call if possible
    if(sigaction(SIGFPE, &sig_fpe, NULL) != 0) {
        printf("Error registering SIGFPE\n");
        exit(1);
    }

    // SIGINT setting
    if(memset(&sig_int, 0, sizeof(sig_int)) != &sig_int) {
        printf("Error from setting memory\n");
        exit(1);
    }
    // setting SIGFPE
    sig_int.sa_handler = sigint_handler;
    sig_int.sa_flags = 0; // restart system call if possible
    if(sigaction(SIGINT, &sig_int, NULL) != 0) {
        printf("Error registering SIGFPE\n");
        exit(1);
    }

    int buffer = 100; // maximum

    char input1[buffer]; // store user's first input
    char input2[buffer]; // store user's second input

    int num1 = 0; // number to be divided
    int num2 = 0; // number to be divide by

    while(1) {
        // get first input
        printf("Enter first integer: ");
        fgets(input1, buffer, stdin);
        num1 = atoi(input1); // convert to int

        // get second input
        printf("Enter second integer: ");
        fgets(input2, buffer, stdin);
        num2 = atoi(input2); // convert to int

	// print the result
        printf("%d / %d is %d with a remainder of %d\n",
			 num1, num2, num1/num2, num1%num2);

        count++; // increment the counter
    }
    return 0;
}
