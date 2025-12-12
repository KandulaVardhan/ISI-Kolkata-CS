#ifndef TREE_H
#define TREE_H

#include "common.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int DATA;

typedef struct tnode {
    DATA data;
    int left;      // index of left child, -1 if none
    int right;     // index of right child, -1 if none
    int parent;    // index of parent, -1 if root
} TNODE;

typedef struct {
    int root;             // index of root node, -1 if empty
    unsigned int size;    // number of nodes currently in use
    unsigned int capacity;// total allocated nodes
    int free;             // index of next free node
    TNODE *nodes;         // array of nodes
} TREE;

// ==== Core Tree Operations ====
void init_tree(TREE *t, int n);
void read_tree(TREE *t);
void free_tree(TREE *t);
int tree_insert(TREE *t, DATA d, int parent_idx, int is_left);
int tree_height(TREE *t, int idx);
void inorder(TREE *t, int idx);
int print_level_order(TREE *t);

// ==== (Optional Future Extensions) ====
int delete_tree(TREE *t, int index, DATA *d);
int find_index(TREE *t, DATA d);
int find_data(TREE *t, int index);
int path_sum(TREE *t, DATA target);

#ifdef __cplusplus
}
#endif

#endif /* TREE_H */
