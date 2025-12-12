#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// ===== Helper: allocate a new node =====
static int new_node(BST *t, DATA d) {
    if (t->free == -1) {
        printf("No free nodes available.\n");
        return -1;
    }

    int idx = t->free;
    t->free = t->nodes[idx].parent; // next free node

    t->nodes[idx].data = d;
    t->nodes[idx].left = -1;
    t->nodes[idx].right = -1;
    t->nodes[idx].parent = -1;
    t->size++;
    return idx;
}

// ===== Initialize BST =====
void init_bst(BST *t, int n) {
    t->nodes = (TNODE *)malloc(sizeof(TNODE) * n);
    assert(t->nodes != NULL);

    t->capacity = n;
    t->size = 0;
    t->root = -1;

    // setup free list using parent field
    for (int i = 0; i < n - 1; i++)
        t->nodes[i].parent = i + 1;
    t->nodes[n - 1].parent = -1;
    t->free = 0;
}

// ===== Insert into BST ===== 
int bst_insert(BST *t, DATA d) {
    // Check if we have space
    if (t->free == -1) {
        printf("Error: No free space left in BST (capacity reached)\n");
        return -1;
    }

    // Allocate a new node from free list
    int idx = t->free;
    t->free = t->nodes[idx].parent; // Move free pointer to next free
    t->nodes[idx].data = d;
    t->nodes[idx].left = -1;
    t->nodes[idx].right = -1;
    t->nodes[idx].parent = -1;
    t->size++;

    // If tree empty, this is the root
    if (t->root == -1) {
        t->root = idx;
        return idx;
    }

    // Normal BST insert traversal
    int curr = t->root, parent = -1;
    while (curr != -1) {
        parent = curr;
        if (d < t->nodes[curr].data)
            curr = t->nodes[curr].left;
        else if (d > t->nodes[curr].data)
            curr = t->nodes[curr].right;
        else {
            // duplicate found, return node to free list
            t->nodes[idx].parent = t->free;
            t->free = idx;
            t->size--;
            printf("Duplicate value not allowed: %d\n", d);
            return -1;
        }
    }

    // Link with parent
    t->nodes[idx].parent = parent;
    if (d < t->nodes[parent].data)
        t->nodes[parent].left = idx;
    else
        t->nodes[parent].right = idx;

    return idx;
}

// ===== Read BST from input file or stdin =====
void read_bst(BST *t) {
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Could not read number of nodes.\n");
        exit(1);
    }

    if (n > t->capacity) {
        fprintf(stderr, "Error: BST capacity (%u) < input size (%d)\n", t->capacity, n);
        exit(1);
    }

    // Read node data and children
    for (int i = 0; i < n; i++) {
        int data, l, r;
        if (scanf("%d %d %d", &data, &l, &r) != 3) {
            fprintf(stderr, "Error reading node %d\n", i);
            exit(1);
        }

        t->nodes[i].data = data;
        t->nodes[i].left = l;
        t->nodes[i].right = r;
        t->nodes[i].parent = -1;  // will fix later
    }

    // Set parent links
    for (int i = 0; i < n; i++) {
        int l = t->nodes[i].left;
        int r = t->nodes[i].right;
        if (l != -1) t->nodes[l].parent = i;
        if (r != -1) t->nodes[r].parent = i;
    }

    // Find root (node with no parent)
    t->root = -1;
    for (int i = 0; i < n; i++) {
        if (t->nodes[i].parent == -1) {
            t->root = i;
            break;
        }
    }

    if (t->root == -1) {
        fprintf(stderr, "Error: Could not find root node.\n");
        exit(1);
    }

    // Update size
    t->size = n;

    // Rebuild free list for unused nodes
    if (n < t->capacity) {
        t->free = n;
        for (int i = n; i < t->capacity - 1; i++) {
            t->nodes[i].parent = i + 1;  // use parent as next free
            t->nodes[i].left = t->nodes[i].right = -1;
        }
        t->nodes[t->capacity - 1].parent = -1;
    } else {
        t->free = -1; // no free slots
    }
}

int find_index(BST *t, DATA d) {
    for (int i = 0; i < t->capacity; i++) {
        if (t->nodes[i].parent != -2 && t->nodes[i].data == d) // parent == -2 means deleted
            return i;
    }
    return -1;
}

// ===== Search BST =====
int bst_search(BST *t, DATA d) {
    int curr = t->root;
    while (curr != -1) {
        if (t->nodes[curr].data == d) return curr;
        if (d < t->nodes[curr].data)
            curr = t->nodes[curr].left;
        else
            curr = t->nodes[curr].right;
    }
    return -1;
}

// ===== Find Minimum Node Index =====
int bst_min(BST *t) {
    if (t->root == -1) return -1;
    int curr = t->root;
    while (t->nodes[curr].left != -1)
        curr = t->nodes[curr].left;
    return t->nodes[curr].data;
}

// ===== Find Maximum Node Index =====
int bst_max(BST *t) {
    if (t->root == -1) return -1;
    int curr = t->root;
    while (t->nodes[curr].right != -1)
        curr = t->nodes[curr].right;
    return t->nodes[curr].data;
}

// ===== Delete Node =====
static int find_min_index(BST *t, int idx) {
    while (t->nodes[idx].left != -1)
        idx = t->nodes[idx].left;
    return idx;
}

int bst_delete(BST *t, DATA d) {
    int curr = bst_search(t, d);
    if (curr == -1) {
        printf("Value not found: %d\n", d);
        return 0;
    }

    int parent = t->nodes[curr].parent;

    // Case 1: leaf node
    if (t->nodes[curr].left == -1 && t->nodes[curr].right == -1) {
        if (parent != -1) {
            if (t->nodes[parent].left == curr) t->nodes[parent].left = -1;
            else t->nodes[parent].right = -1;
        } else {
            t->root = -1;
        }
    }
    // Case 2: one child
    else if (t->nodes[curr].left == -1 || t->nodes[curr].right == -1) {
        int child = (t->nodes[curr].left != -1) ? t->nodes[curr].left : t->nodes[curr].right;
        if (parent != -1) {
            if (t->nodes[parent].left == curr) t->nodes[parent].left = child;
            else t->nodes[parent].right = child;
        } else {
            t->root = child;
        }
        t->nodes[child].parent = parent;
    }
    // Case 3: two children
    else {
        int succ = find_min_index(t, t->nodes[curr].right);
        t->nodes[curr].data = t->nodes[succ].data;
        bst_delete(t, t->nodes[succ].data);
        return 1;
    }

    // Add deleted node back to free list
    t->nodes[curr].parent = t->free;
    t->free = curr;
    t->nodes[curr].parent = -2; // mark deleted
    t->size--;
    return 1;
}

// ===== Inorder Traversal =====
void inorder_bst(BST *t, int idx) {
    if (idx == -1) return;
    inorder_bst(t, t->nodes[idx].left);
    printf("%d ", t->nodes[idx].data);
    inorder_bst(t, t->nodes[idx].right);
}

// ===== Tree Height =====
int bst_height(BST *t, int idx) {
    if (idx == -1) return 0;
    int lh = bst_height(t, t->nodes[idx].left);
    int rh = bst_height(t, t->nodes[idx].right);
    return 1 + (lh > rh ? lh : rh);
}

// ===== Free Memory =====
void free_bst(BST *t) {
    if (t->nodes) free(t->nodes);
    t->nodes = NULL;
    t->size = t->capacity = 0;
    t->root = t->free = -1;
}
