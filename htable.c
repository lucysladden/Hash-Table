#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"
#include <string.h>

/**
 * Here we are declaring our data type and the variables it contains.
 */
struct htablerec{
    unsigned int capacity;
    unsigned int num_keys;
    int *frequencies;
    char **keys;
    int *stats;
    hashing_t method;

};

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    unsigned int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    unsigned int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            } 
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }
    
        fprintf(stream, "%4d %10d %11.1f %10.2f %11d\n", percent_full, 
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n", 
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing"); 
    fprintf(stream, "Percent   Current    Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries    At Home   Collisions   Collisions\n");
    fprintf(stream, "------------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "------------------------------------------------------\n\n");
}

/**
 * This method creates and initialises a hash table 
   with a specific size and collision strategy that passed in as arguments. 
 * And it loops for an input, it reads the input by invoking the getword()
    then it invokes htable_insert() to insert the input to the hash table.
 * It invokes the htable_new() to create an empty hash table.
 * By Default: char array word is set to 256.
 * @param h - the hash table.
 * @param size - the size of the hash table.
 * @param type - the collision hashing strategy.
 * @return h - the new hash table.
 */
htable create_htable(htable h, int size,hashing_t type){
    char word[256];
    h = htable_new(size,type);
    while(getword(word, sizeof word, stdin) != EOF){
        htable_insert(h,word);
    }
    return h;
}


/**
 * Creates and returns an empty hash table with 
   a specific size and collision strategy that passed in as arguments.
 * It allocates memory to all array variables 
   of the hash table as specified in the argument. 
 * @param max - the maximum capacity of the table.
 * @param type - the collision hashing strategy.
 * @return table - the new hash table.
 */

htable htable_new(int max, hashing_t type){
    unsigned int i;
    htable table = emalloc(sizeof *table);
    table->capacity = max;
    table->num_keys = 0;
    table->frequencies = emalloc(table->capacity * sizeof table->frequencies[0]);
    table->keys = emalloc(table->capacity * sizeof table->keys[0]);
    table->stats = emalloc(table->capacity * sizeof table->stats[0]);
    for (i = 0; i < table->capacity; i++){
        table->frequencies[i] = 0;
        table->keys[i] = NULL;
        table->stats[i] = 0;
    }
    table->method = type;
    return table;
}

/**
 * To deallocate all memory allocated in the hash table.
 * @param h - the hashtable.
 */
void htable_free(htable h){
    unsigned int i;
    free(h->frequencies);
    for (i = 0; i < h->capacity; i++){
        free(h->keys[i]);
    }
    free(h->stats);
    free(h);
}

/**
 * Prints out the words and their frequencies.
 * @param freq – the counter of frequencies 
        at that element of the frequencies array.
 * @param word – pointer to the value of the word address.
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}
void htable_print(htable h){
    unsigned int i;
    for (i = 0; i < h->capacity; i++){
        if (h->frequencies[i] > 0){
            print_info(h->frequencies[i], h->keys[i]);
        }
    }
}

/**
 * Displays entire hashtable contents on stderr, showing the values of the
 * following attributes at each value of i.
 *
 * @li Pos – index value i.
 * @li Freq  - number of frequencies at Pos value of frequencies array.
 * @li Stats – stats info at Pos value of stats array.
 * @li Word – value held by key at Pos value in keys array if it exists.
 *
 * @param h - the hashtable to print the contents of the specified values from.
 * @param stream - the stream to send output to.
 *
 */

void htable_print_entire_table(htable h, FILE *stream){
    unsigned int i = 0;
    fprintf(stream, "  Pos  Freq  Stats  Word\n");
    fprintf(stream, "----------------------------------------\n");
    while (i < h->capacity){
        if (h->keys[i] == NULL){
            fprintf(stream, "%5d %5d %5d   %s\n", i, h->frequencies[i], h->stats[i], "");
        } else {
            fprintf(stream, "%5d %5d %5d   %s\n", i, h->frequencies[i], h->stats[i], h->keys[i]);
        }
        i++;
    }

}

/**
 * Converts a string to an integer
 * Note: we use unsigned int type to prevent any issues with negative numbers
   and because we are going to be comparing to other unsigned ints. 
 * @param *word – pointer to the value of the word address.
 */
static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }return result;
}

/**
 * Returns a ‘step’ to pass in to htable_insert that we use to increment
 * from the position where a collision occurs. 
 * Note: this is rather than incrementing by 1.
 * @param h - the hashtable to calculate the step from.
 * @param i_key – value of key at i.
 */
static unsigned int htable_step(htable h, unsigned int i_key){
    return 1 + (i_key % (h->capacity - 1));
}

/**
 * this hashes the string value so it can be inserted into the hashtable by
 * passing in through the *htable_word_to_int() function, and taking the
 * remainder after dividing by capacity.
 *
 * it trys to insert the value at index (the modular of its int value),
 * if this position is occupied, it will increment the index by step (double
 * hashing) or 1 (linear probing). If the value at the index is the same as what
 * is trying to be inserted it will increment value in the corrosponding
 * frequencies array
 * @param h - the hashtable to insert into.
 * @param str – the pointer to address of string value to convert into an index for insertion. 
 */ 
int htable_insert(htable h, char *str){
    unsigned int key = htable_word_to_int(str);
    unsigned int index = key % h->capacity;
    unsigned int collisions = 0;
    unsigned int step = htable_step(h, key);
    while (collisions < h->capacity){
        if(h->keys[index] == NULL){
            h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[index]);
            strcpy(h->keys[index], str);
            h->frequencies[index]++;
            h->stats[h->num_keys] = collisions;
            h->num_keys++;
            return 1;
        } else if ((strcmp(h->keys[index], str)) == 0){
            h->frequencies[index]++;
            return h->frequencies[index];
        } else {
            if(h->method == LINEAR_P){
                index++;
            } else {
                index += step;
            }
            index = index % h->capacity;
            collisions++;
        }
    }
    return 0;
}

