#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdio.h>

typedef struct htablerec *htable;

extern void   htable_free(htable h);
extern int    htable_insert(htable h, char *str);
typedef enum hashing_e { LINEAR_P, DOUBLE_H } hashing_t;
extern htable htable_new(int capacity, hashing_t type);
extern void   htable_print(htable h);
extern int    htable_search(htable h, char *str);
extern void   htable_print_entire_table(htable h, FILE *stream);
extern void htable_print_stats(htable h, FILE *stream, int num_stats);
extern htable create_htable(htable h, int size,hashing_t type);

#endif
