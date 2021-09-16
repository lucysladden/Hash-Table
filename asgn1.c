#include <stdio.h>
#include <stdlib.h>
#include "htable.h"
#include "mylib.h"
#include <getopt.h>


int main(int argc, char **argv) {
    /* By Default:
     * Initialise the htable size to 113.
     * Read Input from stdin.
     * Added to the hash table using htable_insert().
     * Print out keys and their frequenrcies
     * to stdout using htable_print().
     * Use linear probing (As the collision strategy). 
    */
    
    /*Default size of the hash table */
    int size = 113;
    /* Default number of statistical snapshot */
    int n_snapshot = 10;
    /* Number of snapshot that will be passed as input*/
    int input_snapshot = 0;
    /* Default collision hashing strategy */
    hashing_t type = LINEAR_P;
    /* variable to hold the command line arguments */
    char option;
    /* String used as a parameter in getopt() */
    const char *optstring = "deps:t:h";
    /* Default */
    htable h = htable_new(size,type);

    /* Special Case */
    int case_d = 0;

    /* Print case */
    int case_e = 0;
    int case_p = 0;

    /* To store the prime number.*/
    int prime = 0;

    /** Loop to find the command line arguments.
     * It invokes the getopt() to find the input argument in command line.
     * Find the case, if the case is a special case or 
     * a print case, then increment the case variable by 1.
     * If a special case or a print case has been found, 
     * then, break from the switch statement.
     * Because the same multiple argument is not needed.

     * Command line arguments:
        * -d : Use double hashing as the collision resolution strategy.
            (linear probing is the default).
        * -e : Display entire contents of the hash table on stderr using
            format string to display the index, frequency, stats and the key.
        * -p : Print stats info using the function provided in print-stats.txt
            instead of printing the frequencies of words.
        * -s snapshots : Display up to the given number of stats snapshots when
            given -p as an argument. If the table is not full then fewer snapshots will be displayed. 
            Snapshots with 0 entries are not shown.
        * -t tablsize : Use the first prime >= tablesize as the size of your hash table. 
            Assume that tablesize will be a number greater than 0.
        *-h : Print a help message describing how to use the program.
        * If error occurs, then show the error,
        * print the help message and exit the program.
        */
    
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'd':
                if (case_d == 1){
                    break;
                }
                type = DOUBLE_H;
                case_d++;
                break;
            case 'e':
                if(case_e == 1){
                    break;
                }
                case_e++;
                break;
            case 'p':
                if(case_p == 1){
                    break;
                }
                case_p++;
                break;
            case 's':
                input_snapshot = atoi(optarg);
                /*Input snapshot should be greater than 0. */
                if(input_snapshot > 0){ 
                    n_snapshot = input_snapshot;
                }
                break;
            case 't':
                prime = atoi(optarg);
                if(prime >= 0){
                    size = smallest_prime(prime);
                }
                break;
            case 'h':
                print_usage();
                return EXIT_SUCCESS;
            default:
                fprintf(stderr,"\n");
                print_usage();
                return EXIT_FAILURE;
        }
    }

    /* Create hash table. */
    h = create_htable(h,size,type);
    /* If -e is passed as argument, then print entire table */
    if(case_e == 1){
        htable_print_entire_table(h,stderr);
    }
    /** 
     * If -p is passed as argument, then print stats.
     * else, print keywords and frequencies.
    */
    if(case_p == 1){
        htable_print_stats(h,stdout,n_snapshot);
    }else{
        htable_print(h);
    }

    /* Deallocate the memory.*/
    htable_free(h);
    return EXIT_SUCCESS;
}
