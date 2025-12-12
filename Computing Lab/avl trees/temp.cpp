#include <bits/stdc++.h>
using namespace std;

#define FAILURE -1
#define SUCCESS 0
#define E_DUPLICATE 1

struct AVLNode {
    int data;
    int left, right, parent;
    int height;
    //constructor
    AVLNode() : data(0), left(-1), right(-1), parent(-1), height(0) {}
};

class AVLTree {
public:
    int root;
    int num_nodes;
    int max_nodes;
    int free_list;
    vector<AVLNode> nodelist;
    //constructor
    AVLTree(int capacity = 21) {
        init(capacity);
    }

    int init(int capacity) {
        num_nodes = 0;
        max_nodes = capacity;
        root = -1;
        free_list = 0;
        nodelist.resize(max_nodes);
        for (int i = 1; i < max_nodes; ++i)
            nodelist[i - 1].left = i;
        nodelist[max_nodes - 1].left = -1;
        return SUCCESS;
    }

    void read_tree_from_data(){
        int n;
        cin>>n;
        for(int i=0;i<n;i++){
            int temp;
            cin>>temp;
            insert(-1, &root, temp);
        }
    }
    
    void read_tree_array() {
        cin >> num_nodes;
        root = 0;
        max_nodes = max(max_nodes, num_nodes);
        nodelist.resize(max_nodes);

        for (int i = 0; i < num_nodes; ++i) {
            int DATA, LEFT, RIGHT;
            cin >> DATA >> LEFT >> RIGHT;

            nodelist[i].data = DATA;
            nodelist[i].left = LEFT;
            nodelist[i].right = RIGHT;
            nodelist[i].parent = -1;   // will be fixed later
            nodelist[i].height = 0;    // will be fixed later
        }

        // recompute freelist
        free_list = (num_nodes < max_nodes) ? num_nodes : -1;
        for (int i = num_nodes; i < max_nodes - 1; ++i)
            nodelist[i].left = i + 1;
        if (free_list != -1)
            nodelist[max_nodes - 1].left = -1;

        // Compute parent & height info after reading
        compute_parents(root, -1);
        compute_heights(root);
    }

    void compute_parents(int node, int parent) {
        if (node == -1) return;
        nodelist[node].parent = parent;
        compute_parents(nodelist[node].left, node);
        compute_parents(nodelist[node].right, node);
    }

    int compute_heights(int node) {
        if (node == -1) return -1;
        int hl = compute_heights(nodelist[node].left);
        int hr = compute_heights(nodelist[node].right);
        nodelist[node].height = 1 + max(hl, hr);
        return nodelist[node].height;
    }

    int grow_tree() {
        int old_size = max_nodes;
        max_nodes *= 2;
        nodelist.resize(max_nodes);
        free_list = old_size;
        for (int i = old_size + 1; i < max_nodes; ++i)
            nodelist[i - 1].left = i;
        nodelist[max_nodes - 1].left = -1;
        return SUCCESS;
    }

    int get_new_node() {
        if (free_list == -1) {
            if (grow_tree() == FAILURE) return FAILURE;
        }
        int newnode = free_list;
        free_list = nodelist[free_list].left;
        nodelist[newnode].left = nodelist[newnode].right = -1;
        nodelist[newnode].height = 0;
        return newnode;
    }

    void free_up_node(int index) {
        nodelist[index].left = free_list;
        free_list = index;
    }

    int heightOf(int idx) const {
        return idx == -1 ? -1 : nodelist[idx].height;
    }

    int find_successor(int node) const {
        int cur = nodelist[node].right;
        while (cur != -1 && nodelist[cur].left != -1)
            cur = nodelist[cur].left;
        return cur;
    }

    void rotate_LL(int parent, int *node) {
        int k2 = *node;
        int k1 = nodelist[k2].left;
        int Y = nodelist[k1].right;

        nodelist[k2].left = Y;
        nodelist[k1].right = k2;

        if (Y != -1) nodelist[Y].parent = k2;
        nodelist[k1].parent = parent;
        nodelist[k2].parent = k1;

        nodelist[k2].height = 1 + max(heightOf(nodelist[k2].left), heightOf(nodelist[k2].right));
        nodelist[k1].height = 1 + max(heightOf(nodelist[k1].left), heightOf(k2));
        *node = k1;
    }

    void rotate_RR(int parent, int *node) {
        int k1 = *node;
        int k2 = nodelist[k1].right;
        int Y = nodelist[k2].left;

        nodelist[k1].right = Y;
        nodelist[k2].left = k1;

        if (Y != -1) nodelist[Y].parent = k1;
        nodelist[k2].parent = parent;
        nodelist[k1].parent = k2;

        nodelist[k1].height = 1 + max(heightOf(nodelist[k1].left), heightOf(nodelist[k1].right));
        nodelist[k2].height = 1 + max(heightOf(nodelist[k2].left), heightOf(nodelist[k2].right));
        *node = k2;
    }

    void rotate_LR(int parent, int *node) {
        int k3 = *node;
        int k1 = nodelist[k3].left;
        int k2 = nodelist[k1].right;

        nodelist[k1].right = nodelist[k2].left;
        nodelist[k3].left = nodelist[k2].right;
        nodelist[k2].left = k1;
        nodelist[k2].right = k3;

        nodelist[k1].parent = k2;
        nodelist[k3].parent = k2;
        nodelist[k2].parent = parent;

        nodelist[k1].height = 1 + max(heightOf(nodelist[k1].left), heightOf(nodelist[k1].right));
        nodelist[k3].height = 1 + max(heightOf(nodelist[k3].left), heightOf(nodelist[k3].right));
        nodelist[k2].height = 1 + max(heightOf(k1), heightOf(k3));

        *node = k2;
    }

    void rotate_RL(int parent, int *node) {
        int k1 = *node;
        rotate_LL(k1, &nodelist[k1].right);
        rotate_RR(parent, node);
    }

    void balance(int parent, int *node) {
        int thisnode = *node;
        if (thisnode == -1) return;

        int left = nodelist[thisnode].left;
        int right = nodelist[thisnode].right;
        int hl = heightOf(left), hr = heightOf(right);

        if (hl - hr > 1) {
            if (heightOf(nodelist[left].left) >= heightOf(nodelist[left].right))
                rotate_LL(parent, node);
            else
                rotate_LR(parent, node);
        } else if (hr - hl > 1) {
            if (heightOf(nodelist[right].right) >= heightOf(nodelist[right].left))
                rotate_RR(parent, node);
            else
                rotate_RL(parent, node);
        }

        thisnode = *node;
        if (thisnode != -1)
            nodelist[thisnode].height = 1 + max(heightOf(nodelist[thisnode].left),
                                                heightOf(nodelist[thisnode].right));
    }

    int insert_inversioncount(int parent, int *root, int temp, int inversion) {
        int thisnode = *root;

        if (thisnode == -1) {
            int newnode = get_new_node();
            nodelist[newnode].data = temp;
            nodelist[newnode].parent = parent;
            *root = newnode;
            num_nodes++;
            return SUCCESS;
        }

        if (temp < nodelist[thisnode].data) {
            inversion += 1 + size(nodelist[thisnode].right) + insert_inversioncount(thisnode, &nodelist[thisnode].left, temp, inversion);
        } 
        else if (temp > nodelist[thisnode].data) {
            insert_inversioncount(thisnode, &nodelist[thisnode].right, temp, inversion);
        } 
        else {
            return E_DUPLICATE;
        }

        balance(parent, root);
        return inversion;
    }

    int insert(int parent, int *root, int data) {
        int thisnode = *root;
        if (thisnode == -1) {
            int newnode = get_new_node();
            nodelist[newnode].data = data;
            nodelist[newnode].parent = parent;
            *root = newnode;
            num_nodes++;
            return SUCCESS;
        }
        if (data < nodelist[thisnode].data)
            insert(thisnode, &nodelist[thisnode].left, data);
        else if (data > nodelist[thisnode].data)
            insert(thisnode, &nodelist[thisnode].right, data);
        else
            return E_DUPLICATE;

        balance(parent, root);
        return SUCCESS;
    }

    int remove(int parent, int *root, int data) {
        int thisnode = *root;
        if (thisnode == -1) return SUCCESS;

        if (data < nodelist[thisnode].data)
            remove(thisnode, &nodelist[thisnode].left, data);
        else if (data > nodelist[thisnode].data)
            remove(thisnode, &nodelist[thisnode].right, data);
        else {
            if (nodelist[thisnode].left != -1 && nodelist[thisnode].right != -1) {
                int succ = find_successor(thisnode);
                nodelist[thisnode].data = nodelist[succ].data;
                remove(thisnode, &nodelist[thisnode].right, nodelist[succ].data);
            } else {
                int child = (nodelist[thisnode].left != -1) ? nodelist[thisnode].left : nodelist[thisnode].right;
                *root = child;
                if (child != -1) nodelist[child].parent = parent;
                free_up_node(thisnode);
                num_nodes--;
            }
        }

        if (*root != -1) balance(parent, root);
        return SUCCESS;
    }

    int search(int root, int data) const {
        if (root == -1) return -1;
        if (data < nodelist[root].data)
            return search(nodelist[root].left, data);
        else if (data > nodelist[root].data)
            return search(nodelist[root].right, data);
        else
            return root;
    }
    //RECURSIVE
    void inorder(int root) const {
        if (root == -1) return;
        inorder(nodelist[root].left);
        cout << nodelist[root].data << " ";
        inorder(nodelist[root].right);
    }

    void print_tree(int root, int indent = 0) const {
        if (root == -1) return;
        if (nodelist[root].right != -1)
            print_tree(nodelist[root].right, indent + 4);
        if (indent) cout << string(indent, ' ');
        cout << nodelist[root].data << "\n";
        if (nodelist[root].left != -1)
            print_tree(nodelist[root].left, indent + 4);
    }

    int size(int idx) const {
        if (idx == -1) return 0;
        return 1 + size(nodelist[idx].left) + size(nodelist[idx].right);
    }

    int floor_key(int root, int k) const {
        int res = -1;
        while (root != -1) {
            if (nodelist[root].data == k)
                return nodelist[root].data; // exact match
            else if (nodelist[root].data < k) {
                res = nodelist[root].data; // possible candidate
                root = nodelist[root].right; // look for larger but still ≤ k
            } else {
                root = nodelist[root].left;
            }
        }
        return res; // -1 if no such element
    }

    int ceil_key(int root, int k) const {
        int res = -1;
        while (root != -1) {
            if (nodelist[root].data == k)
                return nodelist[root].data;
            else if (nodelist[root].data > k) {
                res = nodelist[root].data;
                root = nodelist[root].left;
            } else {
                root = nodelist[root].right;
            }
        }
        return res;
    }

    int rank_of(int root, int k) const {
        int rank = 0;
        while (root != -1) {
            if (k <= nodelist[root].data) {
                root = nodelist[root].left;
            } else {
                rank += 1 + size(nodelist[root].left);
                root = nodelist[root].right;
            }
        }
        return rank;
    }


};
