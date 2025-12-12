#ifndef BST_H
#define BST_H

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
} BST;

// ==== Core BST Operations ====
void init_bst(BST *t, int n);
void read_bst(BST *t);
void free_bst(BST *t);

// ==== BST Insert / Search / Delete ====
int bst_insert(BST *t, DATA d);
int bst_search(BST *t, DATA d);
int bst_delete(BST *t, DATA d);

// ==== Utility Traversals & Info ====
void inorder_bst(BST *t, int idx);
int bst_height(BST *t, int idx);
int bst_min(BST *t);
int bst_max(BST *t);

// ==== Optional ====
int bst_path_sum(BST *t, DATA target); // optional same as earlier

#ifdef __cplusplus
}
#endif

#endif /* BST_H */
