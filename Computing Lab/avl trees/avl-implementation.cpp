#include <bits/stdc++.h>
using namespace std;

#define FAILURE -1
#define SUCCESS 0
#define E_DUPLICATE 1

struct AVLNode {
    int data;
    int left, right, parent;
    int height;         // height of node (leaf = 0)
    int subtree_size;   // number of nodes in subtree (including itself)

    AVLNode() : data(0), left(-1), right(-1), parent(-1), height(0), subtree_size(1) {}
};

class AVLTree {
public:
    int root;
    int num_nodes;
    int max_nodes;
    int free_list;
    vector<AVLNode> nodelist;

    // constructor
    AVLTree(int capacity = 64) {
        init(capacity);
    }

    int init(int capacity) {
        num_nodes = 0;
        max_nodes = max(1, capacity);
        root = -1;
        free_list = 0;
        nodelist.clear();
        nodelist.resize(max_nodes);
        // build free list via left field
        for (int i = 0; i < max_nodes - 1; ++i)
            nodelist[i].left = i + 1;
        nodelist[max_nodes - 1].left = -1;
        return SUCCESS;
    }

    // --------- reading helpers ----------
    // read N then N lines: DATA LEFT RIGHT (indices), 0-based indices for nodes
    // root will be determined (first node with parent == -1)
    void read_tree_array() {
        int n;
        if (!(cin >> n)) return;
        // ensure capacity
        if (n > max_nodes) {
            max_nodes = n;
            nodelist.resize(max_nodes);
        }
        num_nodes = n;
        // temporary set parent to -1
        for (int i = 0; i < num_nodes; ++i) {
            int DATA, LEFT, RIGHT;
            cin >> DATA >> LEFT >> RIGHT;
            nodelist[i].data = DATA;
            nodelist[i].left = LEFT;
            nodelist[i].right = RIGHT;
            nodelist[i].parent = -1;    // will fill next
            nodelist[i].height = 0;     // will fill
            nodelist[i].subtree_size = 1; // temp
        }
        // compute parents (scan children)
        compute_parents_all();
        // compute heights and subtree sizes
        // find root (node with parent == -1)
        root = -1;
        for (int i = 0; i < num_nodes; ++i) {
            if (nodelist[i].parent == -1) { root = i; break; }
        }
        if (root == -1 && num_nodes > 0) root = 0; // fallback
        compute_heights_and_sizes(root);

        // rebuild free_list from num_nodes..max_nodes-1
        free_list = (num_nodes < max_nodes) ? num_nodes : -1;
        for (int i = num_nodes; i < max_nodes - 1; ++i) nodelist[i].left = i + 1;
        if (free_list != -1) nodelist[max_nodes - 1].left = -1;
    }

    void compute_parents_all() {
        for (int i = 0; i < num_nodes; ++i) nodelist[i].parent = -1;
        for (int i = 0; i < num_nodes; ++i) {
            int L = nodelist[i].left;
            int R = nodelist[i].right;
            if (L != -1) nodelist[L].parent = i;
            if (R != -1) nodelist[R].parent = i;
        }
    }

    int compute_heights_and_sizes(int node) {
        if (node == -1) return -1;
        int hl = compute_heights_and_sizes(nodelist[node].left);
        int hr = compute_heights_and_sizes(nodelist[node].right);
        nodelist[node].height = 1 + max(hl, hr);
        nodelist[node].subtree_size = 1 + ((nodelist[node].left == -1) ? 0 : nodelist[nodelist[node].left].subtree_size)
                                         + ((nodelist[node].right == -1) ? 0 : nodelist[nodelist[node].right].subtree_size);
        return nodelist[node].height;
    }

    // ---------- pool management ----------
    int grow_tree() {
        int old_size = max_nodes;
        int new_size = max_nodes * 2;
        if (new_size < max_nodes + 1) new_size = max_nodes + 1;
        nodelist.resize(new_size);
        // chain new free nodes
        for (int i = old_size; i < new_size - 1; ++i)
            nodelist[i].left = i + 1;
        nodelist[new_size - 1].left = -1;
        free_list = old_size;
        max_nodes = new_size;
        return SUCCESS;
    }

    int get_new_node() {
        if (free_list == -1) {
            grow_tree();
        }
        int idx = free_list;
        free_list = nodelist[idx].left;
        // initialize node
        nodelist[idx].data = 0;
        nodelist[idx].left = nodelist[idx].right = -1;
        nodelist[idx].parent = -1;
        nodelist[idx].height = 0;
        nodelist[idx].subtree_size = 1;
        return idx;
    }

    void free_up_node(int index) {
        // push back onto free list
        nodelist[index].left = free_list;
        free_list = index;
    }

    // ---------- helpers ----------
    int heightOf(int idx) { return idx == -1 ? -1 : nodelist[idx].height; }
    int get_size(int idx) { return idx == -1 ? 0 : nodelist[idx].subtree_size; }

    void update_node(int idx) {
        if (idx == -1) return;
        int L = nodelist[idx].left, R = nodelist[idx].right;
        nodelist[idx].height = 1 + max(heightOf(L), heightOf(R));
        nodelist[idx].subtree_size = 1 + get_size(L) + get_size(R);
    }

    int balance_factor(int idx) {
        if (idx == -1) return 0;
        return heightOf(nodelist[idx].left) - heightOf(nodelist[idx].right);
    }

    // ---------- rotations ----------
    // node is pointer to index variable holding root idx of subtree
    void rotate_LL(int parent, int *node) {
        int k2 = *node;
        int k1 = nodelist[k2].left;
        int Y  = nodelist[k1].right;

        // perform rotation
        nodelist[k2].left = Y;
        if (Y != -1) nodelist[Y].parent = k2;

        nodelist[k1].right = k2;
        // update parents
        nodelist[k1].parent = parent;
        nodelist[k2].parent = k1;

        // update heights and sizes bottom-up
        update_node(k2);
        update_node(k1);

        *node = k1;
    }

    void rotate_RR(int parent, int *node) {
        int k1 = *node;
        int k2 = nodelist[k1].right;
        int Y  = nodelist[k2].left;

        nodelist[k1].right = Y;
        if (Y != -1) nodelist[Y].parent = k1;

        nodelist[k2].left = k1;
        nodelist[k2].parent = parent;
        nodelist[k1].parent = k2;

        update_node(k1);
        update_node(k2);

        *node = k2;
    }

    void rotate_LR(int parent, int *node) {
        // left-right: first rotate right at left child, then left at node
        int k3 = *node;
        int leftChild = nodelist[k3].left;
        // rotate right at left child
        rotate_RR(k3, &nodelist[k3].left);
        // now rotate left at node
        rotate_LL(parent, node);
    }

    void rotate_RL(int parent, int *node) {
        int k1 = *node;
        // rotate left at right child
        rotate_LL(k1, &nodelist[k1].right);
        // rotate right at node
        rotate_RR(parent, node);
    }

    void balance(int parent, int *node) {
        if (*node == -1) return;
        update_node(*node);
        int bf = balance_factor(*node);
        if (bf > 1) {
            int left = nodelist[*node].left;
            if (balance_factor(left) >= 0) {
                rotate_LL(parent, node);
            } else {
                rotate_LR(parent, node);
            }
        } else if (bf < -1) {
            int right = nodelist[*node].right;
            if (balance_factor(right) <= 0) {
                rotate_RR(parent, node);
            } else {
                rotate_RL(parent, node);
            }
        }
        // ensure updated
        if (*node != -1) update_node(*node);
    }

    // ---------- insert (normal) ----------
    int insert(int parent, int *root, int key) {
        if (*root == -1) {
            int nd = get_new_node();
            nodelist[nd].data = key;
            nodelist[nd].parent = parent;
            nodelist[nd].left = nodelist[nd].right = -1;
            nodelist[nd].height = 0;
            nodelist[nd].subtree_size = 1;
            *root = nd;
            num_nodes++;
            return SUCCESS;
        }
        int thisnode = *root;
        if (key < nodelist[thisnode].data) {
            insert(thisnode, &nodelist[thisnode].left, key);
            if (nodelist[thisnode].left != -1) nodelist[nodelist[thisnode].left].parent = thisnode;
        } else if (key > nodelist[thisnode].data) {
            insert(thisnode, &nodelist[thisnode].right, key);
            if (nodelist[thisnode].right != -1) nodelist[nodelist[thisnode].right].parent = thisnode;
        } else {
            return E_DUPLICATE;
        }
        update_node(thisnode);
        balance(parent, root);
        return SUCCESS;
    }

    // ---------- insert with inversion count ----------
    // inversion is accumulated in reference 
    // Insert key into AVL tree while counting inversions.
    // 'inversion' is passed by reference to accumulate total inversions.
    int insert_and_count(int parent, int *root, int key, long long &inversion) {
        if (*root == -1) {
            int nd = get_new_node();
            nodelist[nd].data = key;
            nodelist[nd].parent = parent;
            nodelist[nd].left = nodelist[nd].right = -1;
            nodelist[nd].height = 0;
            nodelist[nd].subtree_size = 1;
            *root = nd;
            num_nodes++;
            return SUCCESS;
        }

        int node = *root;

        if (key < nodelist[node].data) {
            // All elements in the right subtree + current node are greater than key
            inversion += 1 + get_size(nodelist[node].right);
            insert_and_count(node, &nodelist[node].left, key, inversion);
            if (nodelist[node].left != -1)
                nodelist[nodelist[node].left].parent = node;
        }
        else if (key > nodelist[node].data) {
            insert_and_count(node, &nodelist[node].right, key, inversion);
            if (nodelist[node].right != -1)
                nodelist[nodelist[node].right].parent = node;
        }
        else {
            return E_DUPLICATE;
        }

        update_node(node);
        balance(parent, root);
        return SUCCESS;
    }


    // ---------- find successor (smallest in right subtree) ----------
    int find_successor(int node) {
        if (node == -1) return -1;
        int cur = nodelist[node].right;
        while (cur != -1 && nodelist[cur].left != -1) cur = nodelist[cur].left;
        return cur;
    }

    // ---------- remove ----------
    int remove_node(int parent, int *root, int key) {
        if (*root == -1) return SUCCESS;
        int thisnode = *root;
        if (key < nodelist[thisnode].data) {
            remove_node(thisnode, &nodelist[thisnode].left, key);
            if (nodelist[thisnode].left != -1) nodelist[nodelist[thisnode].left].parent = thisnode;
        } else if (key > nodelist[thisnode].data) {
            remove_node(thisnode, &nodelist[thisnode].right, key);
            if (nodelist[thisnode].right != -1) nodelist[nodelist[thisnode].right].parent = thisnode;
        } else {
            // found
            if (nodelist[thisnode].left != -1 && nodelist[thisnode].right != -1) {
                int succ = find_successor(thisnode);
                // copy successor value
                nodelist[thisnode].data = nodelist[succ].data;
                // delete successor from right subtree
                remove_node(thisnode, &nodelist[thisnode].right, nodelist[succ].data);
                if (nodelist[thisnode].right != -1) nodelist[nodelist[thisnode].right].parent = thisnode;
            } else {
                int child = (nodelist[thisnode].left != -1) ? nodelist[thisnode].left : nodelist[thisnode].right;
                *root = child;
                if (child != -1) nodelist[child].parent = parent;
                free_up_node(thisnode);
                num_nodes--;
                return SUCCESS;
            }
        }
        update_node(thisnode);
        balance(parent, root);
        return SUCCESS;
    }

    // ---------- search ----------
    int search(int root_idx, int key) {
        int cur = root_idx;
        while (cur != -1) {
            if (key == nodelist[cur].data) return cur;
            else if (key < nodelist[cur].data) cur = nodelist[cur].left;
            else cur = nodelist[cur].right;
        }
        return -1;
    }

    // ---------- traversals ----------
    void inorder(int root_idx) {
        if (root_idx == -1) return;
        inorder(nodelist[root_idx].left);
        cout << nodelist[root_idx].data << " ";
        inorder(nodelist[root_idx].right);
    }

    // iterative inorder (stack) - optional
    void inorder_iterative(int root_idx) {
        stack<int> st;
        int cur = root_idx;
        while (cur != -1 || !st.empty()) {
            while (cur != -1) {
                st.push(cur);
                cur = nodelist[cur].left;
            }
            cur = st.top(); st.pop();
            cout << nodelist[cur].data << " ";
            cur = nodelist[cur].right;
        }
    }

    // print tree sideways
    void print_tree(int root_idx, int indent = 0) {
        if (root_idx == -1) return;
        if (nodelist[root_idx].right != -1)
            print_tree(nodelist[root_idx].right, indent + 4);
        if (indent) cout << string(indent, ' ');
        cout << nodelist[root_idx].data << " (h=" << nodelist[root_idx].height
             << " sz=" << nodelist[root_idx].subtree_size << ")\n";
        if (nodelist[root_idx].left != -1)
            print_tree(nodelist[root_idx].left, indent + 4);
    }

    // ---------- size using subtree_size ----------
    int size(int idx) {
        return get_size(idx);
    }

    // ---------- floor, ceil, rank ----------
    // floor: largest key <= k, return INT_MIN if none
    int floor_key(int root_idx, int k) {
        int res = INT_MIN;
        int cur = root_idx;
        while (cur != -1) {
            if (nodelist[cur].data == k) return nodelist[cur].data;
            if (nodelist[cur].data < k) {
                res = nodelist[cur].data;
                cur = nodelist[cur].right;
            } else {
                cur = nodelist[cur].left;
            }
        }
        return (res == INT_MIN ? INT_MIN : res);
    }

    // ceil: smallest key >= k, return INT_MAX if none
    int ceil_key(int root_idx, int k) {
        int res = INT_MAX;
        int cur = root_idx;
        while (cur != -1) {
            if (nodelist[cur].data == k) return nodelist[cur].data;
            if (nodelist[cur].data > k) {
                res = nodelist[cur].data;
                cur = nodelist[cur].left;
            } else {
                cur = nodelist[cur].right;
            }
        }
        return (res == INT_MAX ? INT_MAX : res);
    }

    // rank: number of keys strictly less than k
    int rank_of(int root_idx, int k) {
        int rank = 0;
        int cur = root_idx;
        while (cur != -1) {
            if (k <= nodelist[cur].data) {
                cur = nodelist[cur].left;
            } else {
                rank += 1 + get_size(nodelist[cur].left);
                cur = nodelist[cur].right;
            }
        }
        return rank;
    }
}; // end class

