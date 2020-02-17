////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig.c
// This File:        sendsig.c
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

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/** This is the main fuction of this program
 * send signals to a process
 *
 * args - number of arguments
 * argv - array of arguments
 */
int main(int argc, char *argv[]){

    if (argc != 3) { // checking if arguments are in correct format
        printf("Usage: <signal type> <pid>\n");
	exit(1);
    } else {
	int pid = atoi(argv[2]);

        // finds which signal is the correct one to send
	if (strcmp(argv[1],"-i") == 0)
            kill(pid, SIGINT); // send SIGINT signal

	if (strcmp(argv[1],"-u") == 0)
            kill(pid, SIGUSR1); // send SIGUSR1 signal

    }
    return 0;
}
