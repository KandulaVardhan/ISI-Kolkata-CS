#include <bits/stdc++.h>
using namespace std;

#define RED true
#define BLACK false
#define SUCCESS 0
#define E_DUPLICATE 1

struct LLRBNode {
    int data;
    int left, right, parent;
    bool color;         // color of parent link
    int subtree_size;   // count of nodes in subtree

    LLRBNode() : data(0), left(-1), right(-1), parent(-1), color(RED), subtree_size(1) {}
};

class LLRBTree {
public:
    int root;
    int num_nodes;
    int max_nodes;
    int free_list;
    vector<LLRBNode> nodelist;

    LLRBTree(int capacity = 64) { init(capacity); }

    int init(int capacity) {
        num_nodes = 0;
        max_nodes = max(1, capacity);
        root = -1;
        free_list = 0;
        nodelist.clear();
        nodelist.resize(max_nodes);
        // make free list
        for (int i = 0; i < max_nodes - 1; ++i)
            nodelist[i].left = i + 1;
        nodelist[max_nodes - 1].left = -1;
        return SUCCESS;
    }

    // ---------- pool management ----------
    int grow_tree() {
        int old_size = max_nodes;
        int new_size = max_nodes * 2;
        nodelist.resize(new_size);
        for (int i = old_size; i < new_size - 1; ++i)
            nodelist[i].left = i + 1;
        nodelist[new_size - 1].left = -1;
        free_list = old_size;
        max_nodes = new_size;
        return SUCCESS;
    }

    int get_new_node() {
        if (free_list == -1) grow_tree();
        int idx = free_list;
        free_list = nodelist[idx].left;
        nodelist[idx] = LLRBNode();
        return idx;
    }

    void free_up_node(int index) {
        nodelist[index].left = free_list;
        free_list = index;
    }

    // ---------- helpers ----------
    bool isRed(int idx) { return (idx != -1 && nodelist[idx].color == RED); }

    void update_size(int idx) {
        if (idx == -1) return;
        int L = nodelist[idx].left;
        int R = nodelist[idx].right;
        nodelist[idx].subtree_size = 1 +
            (L == -1 ? 0 : nodelist[L].subtree_size) +
            (R == -1 ? 0 : nodelist[R].subtree_size);
    }

    int get_size(int idx) { return idx == -1 ? 0 : nodelist[idx].subtree_size; }

    // ---------- rotations ----------
    int rotateLeft(int h) {
        int x = nodelist[h].right;
        nodelist[h].right = nodelist[x].left;
        if (nodelist[x].left != -1) nodelist[nodelist[x].left].parent = h;
        nodelist[x].left = h;

        nodelist[x].color = nodelist[h].color;
        nodelist[h].color = RED;
        nodelist[x].parent = nodelist[h].parent;
        nodelist[h].parent = x;

        update_size(h);
        update_size(x);
        return x;
    }

    int rotateRight(int h) {
        int x = nodelist[h].left;
        nodelist[h].left = nodelist[x].right;
        if (nodelist[x].right != -1) nodelist[nodelist[x].right].parent = h;
        nodelist[x].right = h;

        nodelist[x].color = nodelist[h].color;
        nodelist[h].color = RED;
        nodelist[x].parent = nodelist[h].parent;
        nodelist[h].parent = x;

        update_size(h);
        update_size(x);
        return x;
    }

    void flipColors(int h) {
        nodelist[h].color = !nodelist[h].color;
        if (nodelist[h].left != -1)
            nodelist[nodelist[h].left].color = !nodelist[nodelist[h].left].color;
        if (nodelist[h].right != -1)
            nodelist[nodelist[h].right].color = !nodelist[nodelist[h].right].color;
    }

    // ---------- insert ----------
    int insertRec(int h, int parent, int key) {
        if (h == -1) {
            int nd = get_new_node();
            nodelist[nd].data = key;
            nodelist[nd].color = RED;
            nodelist[nd].left = nodelist[nd].right = -1;
            nodelist[nd].parent = parent;
            nodelist[nd].subtree_size = 1;
            num_nodes++;
            return nd;
        }

        if (key < nodelist[h].data) {
            int L = insertRec(nodelist[h].left, h, key);
            nodelist[h].left = L;
        } else if (key > nodelist[h].data) {
            int R = insertRec(nodelist[h].right, h, key);
            nodelist[h].right = R;
        } else {
            return h; // duplicate
        }

        // fix-up any right-leaning links
        if (isRed(nodelist[h].right) && !isRed(nodelist[h].left))
            h = rotateLeft(h);
        if (isRed(nodelist[h].left) && isRed(nodelist[nodelist[h].left].left))
            h = rotateRight(h);
        if (isRed(nodelist[h].left) && isRed(nodelist[h].right))
            flipColors(h);

        update_size(h);
        return h;
    }

    void insert(int key) {
        root = insertRec(root, -1, key);
        nodelist[root].color = BLACK; // root always black
    }

    // ---------- search ----------
    int search(int idx, int key) {
        int cur = idx;
        while (cur != -1) {
            if (key == nodelist[cur].data) return cur;
            else if (key < nodelist[cur].data) cur = nodelist[cur].left;
            else cur = nodelist[cur].right;
        }
        return -1;
    }

    // ---------- rank ----------
    int rank_of(int idx, int key) {
        int rank = 0;
        int cur = idx;
        while (cur != -1) {
            if (key <= nodelist[cur].data) {
                cur = nodelist[cur].left;
            } else {
                rank += 1 + get_size(nodelist[cur].left);
                cur = nodelist[cur].right;
            }
        }
        return rank;
    }

    // ---------- floor ----------
    int floor_key(int idx, int key) {
        int res = INT_MIN;
        int cur = idx;
        while (cur != -1) {
            if (nodelist[cur].data == key) return nodelist[cur].data;
            if (nodelist[cur].data < key) {
                res = nodelist[cur].data;
                cur = nodelist[cur].right;
            } else cur = nodelist[cur].left;
        }
        return res;
    }

    // ---------- ceil ----------
    int ceil_key(int idx, int key) {
        int res = INT_MAX;
        int cur = idx;
        while (cur != -1) {
            if (nodelist[cur].data == key) return nodelist[cur].data;
            if (nodelist[cur].data > key) {
                res = nodelist[cur].data;
                cur = nodelist[cur].left;
            } else cur = nodelist[cur].right;
        }
        return res;
    }

    // ---------- size ----------
    int size() { return get_size(root); }

    // ---------- inorder ----------
    void inorder(int idx) {
        if (idx == -1) return;
        inorder(nodelist[idx].left);
        cout << nodelist[idx].data << " ";
        inorder(nodelist[idx].right);
    }

    // ---------- pretty print ----------
    void print_tree(int idx, int indent = 0) {
        if (idx == -1) return;
        if (nodelist[idx].right != -1)
            print_tree(nodelist[idx].right, indent + 4);
        if (indent) cout << string(indent, ' ');
        cout << nodelist[idx].data << (nodelist[idx].color == RED ? " (R)" : " (B)")
             << " sz=" << nodelist[idx].subtree_size << "\n";
        if (nodelist[idx].left != -1)
            print_tree(nodelist[idx].left, indent + 4);
    }
};

// ---------- Example Usage ----------
int main() {
    LLRBTree tree;
    vector<int> keys = {10, 20, 5, 15, 25, 30, 2, 8};
    for (int k : keys) tree.insert(k);

    cout << "Inorder: ";
    tree.inorder(tree.root);
    cout << "\nSize: " << tree.size() << "\n";

    cout << "Rank(15): " << tree.rank_of(tree.root, 15) << "\n";
    cout << "Floor(18): " << tree.floor_key(tree.root, 18) << "\n";
    cout << "Ceil(18): " << tree.ceil_key(tree.root, 18) << "\n\n";

    cout << "Tree structure:\n";
    tree.print_tree(tree.root);
}
