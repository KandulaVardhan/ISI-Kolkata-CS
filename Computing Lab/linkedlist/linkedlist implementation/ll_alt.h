#ifndef LL_ALT_H
#define LL_ALT_H

#include "common.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int DATA;
typedef struct node {
    DATA data;
    int next, prev;
} NODE;

typedef struct {
    int head, tail, free;
    unsigned int length, capacity;
    NODE *elements;
} LIST;

void init_list(LIST *l, int n);  // Declaration only
int insert(LIST *l, unsigned int index, DATA d);
int Delete(LIST *l, unsigned int index, DATA *d);
int find_index_of(LIST *l, DATA d);
int find_value_at(LIST *l, unsigned int index, DATA *d);
void print_list(LIST *l);
void free_list(LIST *l);
void dump_table(LIST *l);

#ifdef __cplusplus
}
#endif

#endif /* LL_ALT_H */
