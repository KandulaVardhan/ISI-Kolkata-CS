#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

class BST {
private:
    struct Node {
        int data;
        int left;
        int right;
        int parent; // -1 = none, -2 = deleted, or used for free list
        Node() : data(0), left(-1), right(-1), parent(-1) {}
    };

    vector<Node> nodes;
    int root;
    int freeList;
    unsigned int size;
    unsigned int capacity;

    int findMinIndex(int idx) {
        if(idx==-1)
        return -1;
        while (nodes[idx].left != -1)
            idx = nodes[idx].left;
        return idx;
    }

    int findMaxIndex(int idx) {
        if(idx==-1)
        return -1;
        while (nodes[idx].right != -1)
            idx = nodes[idx].right;
        return idx;
    }

    void inorder(int idx) {
        if (idx == -1) return;
        inorder(nodes[idx].left);
        cout << nodes[idx].data << " ";
        inorder(nodes[idx].right);
    }

    int height(int idx) {
        if (idx == -1) return 0;
        int lh = height(nodes[idx].left);
        int rh = height(nodes[idx].right);
        return 1 + (lh > rh ? lh : rh);
    }

public:
    // ===== Constructor =====
    BST(unsigned int n) : root(-1), freeList(0), size(0), capacity(n), nodes(n) {
        for (unsigned int i = 0; i < n - 1; i++)
            nodes[i].parent = i + 1;
        nodes[n - 1].parent = -1;
    }

    // ===== Read Tree from Input =====
    void readTree() {
        int n;
        cin >> n;
        if (n > (int)capacity) {
            cerr << "Error: Input exceeds tree capacity (" << capacity << ")\n";
            exit(1);
        }

        for (int i = 0; i < n; i++) {
            int data, l, r;
            cin >> data >> l >> r;
            nodes[i].data = data;
            nodes[i].left = l;
            nodes[i].right = r;
            nodes[i].parent = -1;
        }

        // Set parent pointers
        for (int i = 0; i < n; i++) {
            int l = nodes[i].left;
            int r = nodes[i].right;
            if (l != -1) nodes[l].parent = i;
            if (r != -1) nodes[r].parent = i;
        }

        // Find root
        root = -1;
        for (int i = 0; i < n; i++) {
            if (nodes[i].parent == -1) {
                root = i;
                break;
            }
        }

        if (root == -1) {
            cerr << "Error: Root not found.\n";
            exit(1);
        }

        size = n;

        // Setup free list for unused nodes
        if (n < (int)capacity) {
            freeList = n;
            for (int i = n; i < (int)capacity - 1; i++)
                nodes[i].parent = i + 1;
            nodes[capacity - 1].parent = -1;
        } else {
            freeList = -1;
        }
    }

    // ===== Insert =====
    int insert(int data) {
        if (freeList == -1) {
            cout << "Error: No free space left in BST (capacity reached)\n";
            return -1;
        }

        int idx = freeList;
        freeList = nodes[idx].parent;
        nodes[idx].data = data;
        nodes[idx].left = -1;
        nodes[idx].right = -1;
        nodes[idx].parent = -1;
        size++;

        if (root == -1) {
            root = idx;
            return idx;
        }

        int curr = root, parent = -1;
        while (curr != -1) {
            parent = curr;
            if (data < nodes[curr].data)
                curr = nodes[curr].left;
            else if (data > nodes[curr].data)
                curr = nodes[curr].right;
            else {
                // duplicate
                nodes[idx].parent = freeList;
                freeList = idx;
                size--;
                cout << "Duplicate value not allowed: " << data << "\n";
                return -1;
            }
        }

        nodes[idx].parent = parent;
        if (data < nodes[parent].data)
            nodes[parent].left = idx;
        else
            nodes[parent].right = idx;

        return idx;
    }

    // ===== Search =====
    int search(int data) {
        int curr = root;
        while (curr != -1) {
            if (nodes[curr].data == data)
                return curr;
            if (data < nodes[curr].data)
                curr = nodes[curr].left;
            else
                curr = nodes[curr].right;
        }
        return -1;
    }

    // ===== Delete =====
    bool remove(int data) {
        int curr = search(data);
        if (curr == -1) {
            cout << "Value not found: " << data << "\n";
            return false;
        }

        int parent = nodes[curr].parent;

        // Case 1: leaf
        if (nodes[curr].left == -1 && nodes[curr].right == -1) {
            if (parent != -1) {
                if (nodes[parent].left == curr) nodes[parent].left = -1;
                else nodes[parent].right = -1;
            } else root = -1;
        }
        // Case 2: one child
        else if (nodes[curr].left == -1 || nodes[curr].right == -1) {
            int child = (nodes[curr].left != -1) ? nodes[curr].left : nodes[curr].right;
            if (parent != -1) {
                if (nodes[parent].left == curr) nodes[parent].left = child;
                else nodes[parent].right = child;
            } else root = child;
            nodes[child].parent = parent;
        }
        // Case 3: two children
        else {
            int succ = findMinIndex(nodes[curr].right);
            nodes[curr].data = nodes[succ].data;
            remove(nodes[succ].data);
            return true;
        }

        // Free node
        nodes[curr].parent = freeList;
        freeList = curr;
        size--;
        return true;
    }

    // ===== Utility =====
    int getMin() {
        if (root == -1) return -1;
        return nodes[findMinIndex(root)].data;
    }

    int getMax() {
        if (root == -1) return -1;
        return nodes[findMaxIndex(root)].data;
    }

    int getHeight() { return height(root); }
    unsigned int getSize() { return size; }

    // ===== Print / Traversals =====
    void printInorder() {
        inorder(root);
        cout << "\n";
    }

    // ===== Ancestor / LCA =====
    bool isAncestor(int a, int b) {
        if (a == -1 || b == -1) return false;
        while (b != -1) {
            if (b == a) return true;
            b = nodes[b].parent;
        }
        return false;
    }

    int findLCA(int v1, int v2) {
        int curr = root;
        while (curr != -1) {
            if (v1 < nodes[curr].data && v2 < nodes[curr].data)
                curr = nodes[curr].left;
            else if (v1 > nodes[curr].data && v2 > nodes[curr].data)
                curr = nodes[curr].right;
            else
                return nodes[curr].data; // current node is LCA
        }
        return -1;
    }

    // ===== Count leaves =====
    int countLeaves(int idx) {
        if (idx == -1) return 0;
        if (nodes[idx].left == -1 && nodes[idx].right == -1) return 1;
        return countLeaves(nodes[idx].left) + countLeaves(nodes[idx].right);
    }

    int getLeafCount() { return countLeaves(root); }

    // ===== Check if valid BST =====
    bool isBSTUtil(int idx, int minV, int maxV) {
        if (idx == -1) return true;
        if (nodes[idx].data < minV || nodes[idx].data > maxV) return false;
        return isBSTUtil(nodes[idx].left, minV, nodes[idx].data - 1) &&
               isBSTUtil(nodes[idx].right, nodes[idx].data + 1, maxV);
    }

    bool isBST() { return isBSTUtil(root, INT_MIN, INT_MAX); }

    // ===== Find index by value =====
    int findIndex(int val) { return search(val); }

    // ===== Clear =====
    void clear() {
        nodes.clear();
        nodes.shrink_to_fit();
        root = -1;
        freeList = -1;
        size = 0;
        capacity = 0;
    }

    // ===== Destructor =====
    ~BST() {
        clear();
    }
};
