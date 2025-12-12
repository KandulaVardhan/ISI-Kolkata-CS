#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize a tree with capacity n
void init_tree(TREE *t, int n) {
    t->nodes = (TNODE *)malloc(sizeof(TNODE) * n);
    assert(t->nodes != NULL);

    t->capacity = n;
    t->size = 0;
    t->root = -1;

    // Setup free list
    for (int i = 0; i < n - 1; i++)
        t->nodes[i].parent = i + 1; // use parent field temporarily as 'next free'
    t->nodes[n - 1].parent = -1;
    t->free = 0;
}
// Read tree from input (single tree, not forest)
void read_tree(TREE *t) {
    int n;
    scanf("%d", &n); // number of nodes to read

    if (n > t->capacity) {
        fprintf(stderr, "read_tree: cannot read %d nodes, capacity is %d\n", n, t->capacity);
        exit(1);
    }

    // Update size to reflect number of nodes read
    t->size = n;

    // Initialize read nodes
    for (int i = 0; i < n; i++) {
        int data, l, r;
        scanf("%d %d %d", &data, &l, &r);
        t->nodes[i].data = data;
        t->nodes[i].left = l;
        t->nodes[i].right = r;
        t->nodes[i].parent = -1;
    }

    // Update parent pointers
    for (int i = 0; i < n; i++) {
        int l = t->nodes[i].left;
        int r = t->nodes[i].right;
        if (l != -1) t->nodes[l].parent = i;
        if (r != -1) t->nodes[r].parent = i;
    }

    // Find root (first node with no parent)
    t->root = -1;
    for (int i = 0; i < n; i++) {
        if (t->nodes[i].parent == -1) {
            t->root = i;
            break;
        }
    }

    if (t->root == -1) {
        fprintf(stderr, "read_tree: root not found!\n");
        exit(1);
    }

    // Update free list for remaining capacity
    if (n < t->capacity) {
        t->free = n; // first free slot
        for (int i = n; i < t->capacity - 1; i++) {
            t->nodes[i].left = t->nodes[i].right = t->nodes[i].parent = -1;
            t->nodes[i].parent = i + 1;  // use parent for free chain
        }
        t->nodes[t->capacity - 1].right = -1;
    } else {
        t->free = -1; // no free slots left
    }
}


// Allocate a new node
static int new_node(TREE *t, DATA d) {
    if (t->free == -1) {
        printf("No free nodes available.\n");
        return -1;
    }
    int idx = t->free;
    t->free = t->nodes[idx].parent; // next free index

    t->nodes[idx].data = d;
    t->nodes[idx].left = -1;
    t->nodes[idx].right = -1;
    t->nodes[idx].parent = -1;
    t->size++;
    return idx;
}

// Insert a node (if parent_idx == -1, it becomes root)
int tree_insert(TREE *t, DATA d, int parent_idx, int is_left) {
    int idx = new_node(t, d);
    if (idx == -1)
        return -1;

    if (parent_idx == -1) {
        // No parent -> root node
        if (t->root != -1) {
            printf("Tree already has a root!\n");
            return -1;
        }
        t->root = idx;
        return idx;
    }

    if (is_left) {
        if (t->nodes[parent_idx].left != -1) {
            printf("Left child already exists.\n");
            return -1;
        }
        t->nodes[parent_idx].left = idx;
    } else {
        if (t->nodes[parent_idx].right != -1) {
            printf("Right child already exists.\n");
            return -1;
        }
        t->nodes[parent_idx].right = idx;
    }

    t->nodes[idx].parent = parent_idx;
    return idx;
}

// Inorder traversal
void inorder(TREE *t, int idx) {
    if (idx == -1) return;
    inorder(t, t->nodes[idx].left);
    printf("%d ", t->nodes[idx].data);
    inorder(t, t->nodes[idx].right);
}

// Tree height
int tree_height(TREE *t, int idx) {
    if (idx == -1) return 0;
    int lh = tree_height(t, t->nodes[idx].left);
    int rh = tree_height(t, t->nodes[idx].right);
    return 1 + (lh > rh ? lh : rh);
}

// Level order traversal (simple array-based queue)
int print_level_order(TREE *t) {
    if (t->root == -1) return 0;
    int q[t->size];
    int front = 0, rear = 0;
    q[rear++] = t->root;

    while (front < rear) {
        int idx = q[front++];
        printf("%d ", t->nodes[idx].data);
        if (t->nodes[idx].left != -1) q[rear++] = t->nodes[idx].left;
        if (t->nodes[idx].right != -1) q[rear++] = t->nodes[idx].right;
    }
    printf("\n");
    return 1;
}

// Find node index by data
int find_index(TREE *t, DATA d) {
    for (int i = 0; i < t->capacity; i++) {
        if (t->nodes[i].parent != -2 && t->nodes[i].data == d) // parent == -2 means deleted
            return i;
    }
    return -1;
}

// Find data by index
int find_data(TREE *t, int index) {
    if (index < 0 || index >= (int)t->capacity)
        return 0;
    return t->nodes[index].data;
}

// Delete node by index (only works cleanly for leaf nodes)
int delete_tree(TREE *t, int index, DATA *d) {
    if (index < 0 || index >= (int)t->capacity || t->nodes[index].parent == -2)
        return 0;

    // Only delete if it's a leaf
    if (t->nodes[index].left != -1 || t->nodes[index].right != -1) {
        printf("Can delete only leaf nodes!\n");
        return 0;
    }

    *d = t->nodes[index].data;

    // Unlink from parent
    int p = t->nodes[index].parent;
    if (p != -1) {
        if (t->nodes[p].left == index) t->nodes[p].left = -1;
        else if (t->nodes[p].right == index) t->nodes[p].right = -1;
    } else {
        t->root = -1; // deleted root
    }

    // Add to free list
    t->nodes[index].parent = t->free; // freelist linkage
    t->free = index;
    t->nodes[index].parent = -2; // mark as deleted

    t->size--;
    return 1;
}


// Check if there exists a root-to-leaf path with given sum
static int path_sum_util(TREE *t, int idx, DATA curr_sum, DATA target) {
    if (idx == -1) return 0;

    curr_sum += t->nodes[idx].data;

    if (t->nodes[idx].left == -1 && t->nodes[idx].right == -1)
        return (curr_sum == target);

    return path_sum_util(t, t->nodes[idx].left, curr_sum, target) ||
           path_sum_util(t, t->nodes[idx].right, curr_sum, target);
}

int path_sum(TREE *t, DATA target) {
    if (t->root == -1) return 0;
    return path_sum_util(t, t->root, 0, target);
}

// Free memory
void free_tree(TREE *t) {
    if (t->nodes) free(t->nodes);
    t->nodes = NULL;
    t->size = t->capacity = 0;
    t->root = t->free = -1;
}
