////////////////////////////////////////////////////////////////////////////////
// Main File:        intdate.c
// This File:        intdate.c
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>

// global variables
int count = 0; // the number of times SIGUSR1 is caught

/**
 * Used to handle the SIGALRM singal
 * Print PID, current time for every 3 seconds
 *
 * sig: signal number
 */
void sigalrm_handler(int sig) {

    //calculating current time
    time_t cur;
    time(&cur);

    if (cur == ((time_t)-1) ) {
        fprintf(stderr, "ERROR CALCULATING TIME");
        exit(1);
    }

    //print the pid and current time
    printf("PID: %d | Current Time: %s", getpid(), ctime(&cur));
    alarm(3);
}

/**
 * Used to handle the SIGUSR1 singal
 * Keep track of the number of times the SIGUSR1 caught
 *
 * sig: signal number
 */
void sigusr1_handler(int sig) {
    count++;
    printf("SIGUSR1 caught!\n");
}

/**
 * Used to handle the SIGINT singal
 * Print the total number of SIFUSR1 caught and exit the program
 *
 * sig: signal number
 */
void sigint_handler(int sig) {
    //exit the program
    printf("\nSIGINT received.\n");
    printf("SIGUSR1 was received %d times. Exiting now.\n", count);
    exit(0);
}


/** The main function of program
 * Setting all sigactions and have an infinite loop until terminated
 * Send an alarm signal for every 3 seconds
 *
 * argc - number of arguments
 * argv - array of arguments
 */
int main(int argc, const char *argv[]) {

    // initial message displayed to user
    printf("Pid and time will be printed every 3 seconds.\n");
    printf("Enter ^C to end the program.\n");

    // struct to handle signal actions
    struct sigaction sig_alarm, sig_usr1, sig_int;

    // SIGALRM setting
    if(memset(&sig_alarm, 0, sizeof(sig_alarm)) != &sig_alarm) {
	printf("Error from setting memory\n");
	exit(1);
    }
    // setting SIGALRM
    sig_alarm.sa_handler = sigalrm_handler;
    sig_alarm.sa_flags = 0; // restart system call if possible
    if(sigaction(SIGALRM, &sig_alarm, NULL) != 0) {
	printf("Error registering SIGALRM\n");
	exit(1);
    }

    // SIGUSR1 setting
    if(memset(&sig_usr1, 0, sizeof(sig_usr1)) != &sig_usr1) {
        printf("Error from setting memory\n");
        exit(1);
    }
    // setting SIGALRM
    sig_usr1.sa_handler = sigusr1_handler;
    sig_usr1.sa_flags = 0; // restart system call if possible
    if(sigaction(SIGUSR1, &sig_usr1, NULL) != 0) {
        printf("Error registering SIGUSR1\n");
        exit(1);
    }

    // SIGINT setting
    if(memset(&sig_int, 0, sizeof(sig_int)) != &sig_int) {
        printf("Error from setting memory\n");
        exit(1);
    }
    // setting SIGALRM
    sig_int.sa_handler = sigint_handler;
    sig_int.sa_flags = 0; // restart system call if possible
    if(sigaction(SIGINT, &sig_int, NULL) != 0) {
        printf("Error registering SIGINT\n");
        exit(1);
    }

    alarm(3); // alarm for every 3 seconds

    while(1); // infinite loop

    return 0;
}
