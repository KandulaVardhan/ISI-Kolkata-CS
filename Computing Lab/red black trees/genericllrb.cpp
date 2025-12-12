#include <iostream>
#include <string>
#include <stdexcept>
#include <functional>
using namespace std;

// ============================
// Generic Left-Leaning Red-Black Tree
// ============================
template <typename Key, typename Compare = std::less<Key>>
class LLRB {
private:
    static const bool RED = true;
    static const bool BLACK = false;

    struct Node {
        Key key;
        Node *left, *right;
        bool color;
        int size;
        Node(const Key& k, bool c)
            : key(k), left(nullptr), right(nullptr), color(c), size(1) {}
    };

    Node* root;
    Compare comp;

    // ===== Utility Functions =====
    bool isRed(Node* x) const {
        return x != nullptr && x->color == RED;
    }

    int sizeOf(Node* x) const {
        return x ? x->size : 0;
    }

    void updateSize(Node* x) {
        if (x)
            x->size = 1 + sizeOf(x->left) + sizeOf(x->right);
    }

    // ===== Rotations =====
    Node* rotateLeft(Node* h) {
        Node* x = h->right;
        h->right = x->left;
        x->left = h;
        x->color = h->color;
        h->color = RED;
        x->size = h->size;
        updateSize(h);
        return x;
    }

    Node* rotateRight(Node* h) {
        Node* x = h->left;
        h->left = x->right;
        x->right = h;
        x->color = h->color;
        h->color = RED;
        x->size = h->size;
        updateSize(h);
        return x;
    }

    void flipColors(Node* h) {
        h->color = !h->color;
        if (h->left) h->left->color = !h->left->color;
        if (h->right) h->right->color = !h->right->color;
    }

    // ===== Insertion =====
    Node* insertRec(Node* h, const Key& key) {
        if (h == nullptr) return new Node(key, RED);

        if (comp(key, h->key))
            h->left = insertRec(h->left, key);
        else if (comp(h->key, key))
            h->right = insertRec(h->right, key);
        else
            return h; // duplicate

        if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);
        if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
        if (isRed(h->left) && isRed(h->right)) flipColors(h);

        updateSize(h);
        return h;
    }

    // ===== Search =====
    bool searchRec(Node* h, const Key& key) const {
        if (!h) return false;
        if (comp(key, h->key))
            return searchRec(h->left, key);
        else if (comp(h->key, key))
            return searchRec(h->right, key);
        else
            return true;
    }

    // ===== Min/Max =====
    Node* minNode(Node* h) const {
        if (!h) return nullptr;
        while (h->left) h = h->left;
        return h;
    }

    Node* maxNode(Node* h) const {
        if (!h) return nullptr;
        while (h->right) h = h->right;
        return h;
    }

    // ===== Rank & Select =====
    int rankRec(Node* h, const Key& key) const {
        if (!h) return 0;
        if (comp(key, h->key))
            return rankRec(h->left, key);
        else if (comp(h->key, key))
            return 1 + sizeOf(h->left) + rankRec(h->right, key);
        else
            return sizeOf(h->left);
    }

    Node* selectRec(Node* h, int k) const {
        if (!h) return nullptr;
        int leftSize = sizeOf(h->left);
        if (k < leftSize) return selectRec(h->left, k);
        else if (k > leftSize) return selectRec(h->right, k - leftSize - 1);
        else return h;
    }

    // ===== Traversal =====
    void inorderRec(Node* h) const {
        if (!h) return;
        inorderRec(h->left);
        cout << h->key << "\n";
        inorderRec(h->right);
    }

    // ===== Delete Min =====
    Node* moveRedLeft(Node* h) {
        flipColors(h);
        if (isRed(h->right->left)) {
            h->right = rotateRight(h->right);
            h = rotateLeft(h);
            flipColors(h);
        }
        return h;
    }

    Node* deleteMinRec(Node* h) {
        if (h->left == nullptr)
            return nullptr;
        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h);
        h->left = deleteMinRec(h->left);
        return balance(h);
    }

    // ===== Delete Key =====
    Node* moveRedRight(Node* h) {
        flipColors(h);
        if (isRed(h->left->left)) {
            h = rotateRight(h);
            flipColors(h);
        }
        return h;
    }

    Node* balance(Node* h) {
        if (isRed(h->right)) h = rotateLeft(h);
        if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
        if (isRed(h->left) && isRed(h->right)) flipColors(h);
        updateSize(h);
        return h;
    }

    Node* deleteRec(Node* h, const Key& key) {
        if (comp(key, h->key)) {
            if (h->left) {
                if (!isRed(h->left) && !isRed(h->left->left))
                    h = moveRedLeft(h);
                h->left = deleteRec(h->left, key);
            }
        } else {
            if (isRed(h->left))
                h = rotateRight(h);
            if (!comp(h->key, key) && h->right == nullptr)
                return nullptr;
            if (h->right) {
                if (!isRed(h->right) && !isRed(h->right->left))
                    h = moveRedRight(h);
                if (!comp(h->key, key) && !comp(key, h->key)) {
                    Node* x = minNode(h->right);
                    h->key = x->key;
                    h->right = deleteMinRec(h->right);
                } else {
                    h->right = deleteRec(h->right, key);
                }
            }
        }
        return balance(h);
    }

    void clearRec(Node* h) {
        if (!h) return;
        clearRec(h->left);
        clearRec(h->right);
        delete h;
    }

public:
    LLRB() : root(nullptr) {}

    // ===== Public Interface =====
    void insert(const Key& key) {
        root = insertRec(root, key);
        root->color = BLACK;
    }

    void remove(const Key& key) {
        if (!contains(key)) return;
        if (!isRed(root->left) && !isRed(root->right))
            root->color = RED;
        root = deleteRec(root, key);
        if (root) root->color = BLACK;
    }

    bool contains(const Key& key) const {
        return searchRec(root, key);
    }

    Key getMin() const {
        Node* x = minNode(root);
        if (!x) throw runtime_error("Tree is empty");
        return x->key;
    }

    Key getMax() const {
        Node* x = maxNode(root);
        if (!x) throw runtime_error("Tree is empty");
        return x->key;
    }

    int rank(const Key& key) const {
        return rankRec(root, key);
    }

    Key select(int k) const {
        Node* x = selectRec(root, k);
        if (!x) throw runtime_error("Invalid rank");
        return x->key;
    }

    void printInorder() const {
        inorderRec(root);
    }

    int size() const {
        return sizeOf(root);
    }

    void clear() {
        clearRec(root);
        root = nullptr;
    }

    ~LLRB() {
        clear();
    }
};

// ============================
// Example Struct: Student
// ============================
struct Student {
    int roll;
    string name;

    bool operator<(const Student& other) const {
        return roll < other.roll;
    }
};

ostream& operator<<(ostream& os, const Student& s) {
    os << "{Roll: " << s.roll << ", Name: " << s.name << "}";
    return os;
}

struct StudentCompare {
    bool operator()(const Student& a, const Student& b) const {
        return a.roll < b.roll;
    }
};

// ============================
// Example Usage
// ============================
int main() {
    LLRB<Student, StudentCompare> t;
    t.insert({7, "Alice"});
    t.insert({3, "Bob"});
    t.insert({10, "Charlie"});
    t.insert({5, "David"});

    cout << "Inorder Traversal:\n";
    t.printInorder();

    cout << "\nMin: " << t.getMin() << endl;
    cout << "Max: " << t.getMax() << endl;

    cout << "\nRank of roll=7: " << t.rank({7, ""}) << endl;
    cout << "Select rank=2: " << t.select(2) << endl;

    t.remove({7, ""});
    cout << "\nAfter deleting roll=7:\n";
    t.printInorder();

    return 0;
}
