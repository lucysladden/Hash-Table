#include <stdio.h> /* for fprintf */
#include <stdlib.h> /* for size_t, malloc, realloc, exit */
#include <assert.h> /* for assert */
#include <ctype.h>
#include "mylib.h"


/**
 * Dynamically allocates memory block with specified size.
 * @param s – size of memory block.
 * @return result - the pointer to the allocated memory.
 */
void *emalloc(size_t s) {
    void *result = malloc(s);
    if (NULL == result) {
        fprintf(stderr, "memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;

}

/**
 * Dynamically reallocates more memory by calling realloc
 * @param p – pointer variable of newly allocated memory.
 * @param s – size of new memory block. 
 * @return result - pointer of the reallocated memory.
 */
void *erealloc(void *p, size_t s) {
    void *result = realloc(p, s);
    if (NULL == result) {
        fprintf(stderr, "memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}
/**
 * To get a word from the input stream.
 * @param s - pointer variable of an array of characters.
 * @param limit - the limit number of characters to read.
 *               (The size of the pointer s).
 * @param stream - pointer to the input stream to read from stdin.
 * @return EOF - if the end of the input stream has been reached.
 * @return the number of characters read from stdin.
 * */
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);

    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c);
    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) { /* reduce limit by 1 to allow for the \0 */
        *w++ = tolower(c);
    }while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}

/**
 * Returns the next prime number greater than 
 * or equal to the given number that is passed in as an argument.
 * @param n - the number.
 * @return the prime number.
 */
int smallest_prime(int n){
    /* Assume that n > 0 
    Bertrand's Postulate states that 
     for every n > 1 integer there is always 
     at least one prime such that n < p < 2n.
    */
   int i = 0;
   int count,j;
   if(n == 0 || n == 1){
       return 2;
   }
   
   for(i = n; i < 2 * n; i++){
       count = 0;
       for(j = 2; j < i; j++){
           if(i % j == 0){
               count++;
               break;
           }
       }
       if(count == 0){
           break;
       }
   }
   return i;
}

/**
 * Prints the description of the program and what arguments can be passed to it.
 */
void print_usage(){
    char *str = "Usage: ./program [OPTION]... <STDIN>\n\n"
    "Perform various operations using a hash table.\t By default, words are\n"
    "read from stdin and added to the hash table, before being printed out\n"
    "alongside their frequencies to stdout.\n\n";
    char *str2 = "Command line arguments:\n"
    " -d\t      Use double hashing (linear probing is the default)\n"
    " -e\t      Display entire contents of the hash table on stderr\n"
    " -p\t      Print stats info instead of frequencies & words\n";
    char *str3 = " -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n"
    " -t TABLESIZE Use the first prime >= TABLESIZE as the htable size\n\n"
    " -h\t      Display help message\n\n";
    printf("%s%s%s",str,str2,str3);
}
