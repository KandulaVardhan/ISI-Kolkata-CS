#include <iostream>
#include <vector>
#include <limits>
#include <functional>
#include <string>
using namespace std;

// Example struct for key
struct Student {
    int id;
    string name;

    Student() : id(0), name("") {}
    Student(int i, string n) : id(i), name(n) {}
};

// Comparator for Student
struct StudentCompare {
    bool operator()(const Student& a, const Student& b) const {
        return a.id < b.id;
    }
};

// Generic BST
template <typename Key, typename Value, typename Compare = std::less<Key>>
class BST {
private:
    struct Node {
        Key key;
        Value value;
        int left, right, parent;

        Node() : left(-1), right(-1), parent(-1) {}
        Node(Key k, Value v) : key(k), value(v), left(-1), right(-1), parent(-1) {}
    };

    vector<Node> nodes;
    int root;
    int freeList;
    Compare comp;

    // Get free node index
    int getFreeNode() {
        if (freeList == -1) return -1;
        int idx = freeList;
        freeList = nodes[freeList].parent;
        return idx;
    }

    // Recursive inorder traversal
    void inorder(int idx) {
        if (idx == -1) return;
        inorder(nodes[idx].left);
        cout << "{" << nodes[idx].key.id << ", " << nodes[idx].key.name << "} -> " << nodes[idx].value << endl;
        inorder(nodes[idx].right);
    }

    // Height calculation
    int height(int idx) {
        if (idx == -1) return 0;
        return 1 + max(height(nodes[idx].left), height(nodes[idx].right));
    }

    // Check BST property
    bool isBSTUtil(int idx, const Key& minK, const Key& maxK) {
        if (idx == -1) return true;
        const Key& k = nodes[idx].key;
        if (comp(k, minK) || comp(maxK, k)) return false;
        return isBSTUtil(nodes[idx].left, minK, k) && isBSTUtil(nodes[idx].right, k, maxK);
    }

    // Find ancestor
    int findAncestor(int idx, const Key& key1, const Key& key2) {
        if (idx == -1) return -1;
        const Key& k = nodes[idx].key;

        bool left1 = comp(key1, k);
        bool left2 = comp(key2, k);

        if (left1 && left2) return findAncestor(nodes[idx].left, key1, key2);
        else if (!left1 && !left2) return findAncestor(nodes[idx].right, key1, key2);
        else return idx;
    }

public:
    BST(size_t capacity = 100) {
        nodes.resize(capacity);
        root = -1;
        if (capacity > 0) {
            freeList = 0;
            for (size_t i = 0; i < capacity - 1; i++)
                nodes[i].parent = i + 1;
            nodes[capacity - 1].parent = -1;
        } else freeList = -1;
    }

    void insert(Key key, Value value) {
        int z = getFreeNode();
        if (z == -1) {
            cout << "Tree full!\n";
            return;
        }
        nodes[z] = Node(key, value);

        int y = -1, x = root;
        while (x != -1) {
            y = x;
            if (comp(key, nodes[x].key))
                x = nodes[x].left;
            else
                x = nodes[x].right;
        }

        nodes[z].parent = y;
        if (y == -1)
            root = z;
        else if (comp(key, nodes[y].key))
            nodes[y].left = z;
        else
            nodes[y].right = z;
    }

    bool search(const Key& key, Value& outValue) {
        int x = root;
        while (x != -1) {
            if (comp(key, nodes[x].key))
                x = nodes[x].left;
            else if (comp(nodes[x].key, key))
                x = nodes[x].right;
            else {
                outValue = nodes[x].value;
                return true;
            }
        }
        return false;
    }

    void printInorder() {
        inorder(root);
    }

    int height() {
        return height(root);
    }

    bool isBST() {
        if (root == -1) return true;
        Key minK = nodes[root].key;
        Key maxK = nodes[root].key;
        return isBSTUtil(root, minK, maxK);
    }

    void clear() {
        root = -1;
        if (!nodes.empty()) {
            freeList = 0;
            for (size_t i = 0; i < nodes.size() - 1; i++)
                nodes[i].parent = i + 1;
            nodes.back().parent = -1;
        }
    }

    void findCommonAncestor(const Key& k1, const Key& k2) {
        int idx = findAncestor(root, k1, k2);
        if (idx == -1)
            cout << "No common ancestor found.\n";
        else
            cout << "Common ancestor: {" << nodes[idx].key.id << ", " << nodes[idx].key.name << "}\n";
    }
};

// ---------------- DEMO ----------------
int main() {
    BST<Student, string, StudentCompare> t(10);

    t.insert({1, "Alice"}, "Math");
    t.insert({3, "Charlie"}, "CS");
    t.insert({2, "Bob"}, "Physics");
    t.insert({4, "David"}, "AI");

    cout << "\nInorder traversal:\n";
    t.printInorder();

    cout << "\nHeight: " << t.height() << endl;

    string val;
    if (t.search({2, "Bob"}, val))
        cout << "\nFound student 2: " << val << endl;
    else
        cout << "\nStudent not found\n";

    t.findCommonAncestor({1, "Alice"}, {3, "Charlie"});

    cout << "\nClearing tree...\n";
    t.clear();

    t.printInorder(); // should print nothing
}
