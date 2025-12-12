#include <iostream>
#include <vector>
#include <limits>
#include <stdexcept>
#include <cassert>
using namespace std;

template <typename Key, typename Value>
class BST {
private:
    struct Node {
        Key key;
        Value val;
        int left, right, parent; // parent used as parent while allocated; when freed it's used as free-list link
        Node() : left(-1), right(-1), parent(-1) {}
    };

    vector<Node> nodes;
    int root;
    int freeList;
    unsigned int size;
    unsigned int capacity;

    // ===== Helper functions =====
    int findMinIndex(int idx) const {
        if (idx == -1) return -1;
        while (idx != -1 && nodes[idx].left != -1)
            idx = nodes[idx].left;
        return idx;
    }

    int findMaxIndex(int idx) const {
        if (idx == -1) return -1;
        while (idx != -1 && nodes[idx].right != -1)
            idx = nodes[idx].right;
        return idx;
    }

    void inorder(int idx) const {
        if (idx == -1) return;
        inorder(nodes[idx].left);
        cout << "(" << nodes[idx].key << ", " << nodes[idx].val << ") ";
        inorder(nodes[idx].right);
    }

    int height(int idx) const {
        if (idx == -1) return 0;
        int lh = height(nodes[idx].left);
        int rh = height(nodes[idx].right);
        return 1 + max(lh, rh);
    }

    bool isBSTUtil(int idx, Key minV, Key maxV) const {
        if (idx == -1) return true;
        if (nodes[idx].key < minV || nodes[idx].key > maxV)
            return false;
        return isBSTUtil(nodes[idx].left, minV, nodes[idx].key) &&
               isBSTUtil(nodes[idx].right, nodes[idx].key, maxV);
    }

    int countLeaves(int idx) const {
        if (idx == -1) return 0;
        if (nodes[idx].left == -1 && nodes[idx].right == -1)
            return 1;
        return countLeaves(nodes[idx].left) + countLeaves(nodes[idx].right);
    }

public:
    // ===== Constructor =====
    BST(unsigned int n)
        : root(-1), freeList(0), size(0), capacity(n), nodes(n) {
        for (unsigned int i = 0; i < n - 1; i++)
            nodes[i].parent = i + 1; // build free list via parent field
        nodes[n - 1].parent = -1;
    }

    // ===== Insert =====
    int insert(Key key, Value val) {
        if (freeList == -1) {
            cerr << "Error: Tree capacity reached\n";
            return -1;
        }

        int idx = freeList;                 // allocate from free list
        freeList = nodes[idx].parent;       // next free

        nodes[idx].key = key;
        nodes[idx].val = val;
        nodes[idx].left = nodes[idx].right = -1;
        nodes[idx].parent = -1;             // will set proper parent below
        size++;

        if (root == -1) {
            root = idx;
            return idx;
        }

        int curr = root, parent = -1;
        while (curr != -1) {
            parent = curr;
            if (key < nodes[curr].key)
                curr = nodes[curr].left;
            else if (key > nodes[curr].key)
                curr = nodes[curr].right;
            else {
                // duplicate key: return node to free list
                nodes[idx].parent = freeList;
                freeList = idx;
                size--;
                cerr << "Duplicate key: " << key << endl;
                return -1;
            }
        }

        nodes[idx].parent = parent;
        if (key < nodes[parent].key)
            nodes[parent].left = idx;
        else
            nodes[parent].right = idx;

        return idx;
    }

    // ===== Search =====
    int search(Key key) const {
        int curr = root;
        while (curr != -1) {
            if (nodes[curr].key == key)
                return curr;
            if (key < nodes[curr].key)
                curr = nodes[curr].left;
            else
                curr = nodes[curr].right;
        }
        return -1;
    }

    // ===== Find index (same as search but named) =====
    int findIndex(Key key) const {
        return search(key);
    }

    // ===== Remove =====
    bool remove(Key key) {
        // locate node to delete
        int cur = root;
        int parent = -1;
        bool isLeftChild = false;
        while (cur != -1 && nodes[cur].key != key) {
            parent = cur;
            if (key < nodes[cur].key) {
                isLeftChild = true;
                cur = nodes[cur].left;
            } else {
                isLeftChild = false;
                cur = nodes[cur].right;
            }
        }

        if (cur == -1) {
            cerr << "Key not found: " << key << endl;
            return false;
        }

        // if node has two children, find successor and copy
        if (nodes[cur].left != -1 && nodes[cur].right != -1) {
            int succ = findMinIndex(nodes[cur].right);
            // copy succ data into cur
            nodes[cur].key = nodes[succ].key;
            nodes[cur].val = nodes[succ].val;
            // now we will delete successor instead
            // set cur to succ and parent to succ_parent for deletion below
            // find parent of succ
            int succParent = cur;
            if (nodes[cur].right != succ) {
                // succ is deeper: walk from cur->right to find parent of succ
                succParent = nodes[cur].right;
                while (nodes[succParent].left != succ)
                    succParent = nodes[succParent].left;
            } else {
                succParent = cur;
            }
            // reassign cur and parent to delete succ node
            cur = succ;
            parent = succParent;
            isLeftChild = (nodes[parent].left == cur);
            // continue to deletion code below (cur has at most one child)
        }

        // Now cur has at most one child
        int child = (nodes[cur].left != -1) ? nodes[cur].left : nodes[cur].right;

        if (parent == -1) {
            // deleting root
            root = child;
            if (child != -1) nodes[child].parent = -1;
        } else {
            if (isLeftChild)
                nodes[parent].left = child;
            else
                nodes[parent].right = child;
            if (child != -1) nodes[child].parent = parent;
        }

        // push cur into free list (use parent as free-list link)
        nodes[cur].parent = freeList;
        freeList = cur;
        size--;
        return true;
    }

    // ===== Utilities =====
    Key getMin() const {
        if (root == -1) throw runtime_error("Empty tree");
        int idx = findMinIndex(root);
        return nodes[idx].key;
    }

    Key getMax() const {
        if (root == -1) throw runtime_error("Empty tree");
        int idx = findMaxIndex(root);
        return nodes[idx].key;
    }

    int getHeight() const { return height(root); }
    unsigned int getSize() const { return size; }
    int getLeafCount() const { return countLeaves(root); }

    // ===== Traversal =====
    void printInorder() const {
        inorder(root);
        cout << "\n";
    }

    // ===== Ancestor / LCA =====
    bool isAncestor(int a, int b) const {
        if (a == -1 || b == -1) return false;
        while (b != -1) {
            if (b == a) return true;
            b = nodes[b].parent;
        }
        return false;
    }

    Key findLCA(Key k1, Key k2) const {
        int curr = root;
        while (curr != -1) {
            if (k1 < nodes[curr].key && k2 < nodes[curr].key)
                curr = nodes[curr].left;
            else if (k1 > nodes[curr].key && k2 > nodes[curr].key)
                curr = nodes[curr].right;
            else
                return nodes[curr].key;
        }
        throw runtime_error("LCA not found");
    }

    bool isBST() const {
        if (root == -1) return true;
        return isBSTUtil(root,
                         numeric_limits<Key>::lowest(),
                         numeric_limits<Key>::max());
    }

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
    ~BST() { clear(); }
};

// ===== Example Usage =====
int main() {
    BST<int, string> t(20);

    t.insert(50, "root");
    t.insert(30, "left");
    t.insert(70, "right");
    t.insert(20, "left.left");
    t.insert(40, "left.right");
    t.insert(60, "right.left");
    t.insert(80, "right.right");

    cout << "Inorder: ";
    t.printInorder();
    cout << "Height: " << t.getHeight() << ", Size: " << t.getSize() << "\n";
    cout << "Min: " << t.getMin() << ", Max: " << t.getMax() << "\n";
    cout << "Leaves: " << t.getLeafCount() << "\n";
    cout << "Is BST? " << (t.isBST() ? "Yes" : "No") << "\n";

    int a = t.findIndex(30), b = t.findIndex(40);
    cout << "Is 30 ancestor of 40? " << (t.isAncestor(a, b) ? "Yes" : "No") << "\n";
    cout << "LCA(20, 40): " << t.findLCA(20, 40) << "\n";

    cout << "\nDeleting 20 and 70...\n";
    t.remove(20);
    t.remove(70);
    t.printInorder();

    cout << "\nClearing tree...\n";
    t.clear();
    cout << "Tree cleared.\n";
}
