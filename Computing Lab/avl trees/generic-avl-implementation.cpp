#include <iostream>
#include <queue>
#include <limits>
#include <string>
#include <stdexcept>
#include <functional>
using namespace std;

// ====================== Example Struct + Comparator ======================
struct Student {
    int id;
    double gpa;

    friend ostream& operator<<(ostream& os, const Student& s) {
        return os << "{id:" << s.id << ", gpa:" << s.gpa << "}";
    }
};

struct StudentCompare {
    bool operator()(const Student& a, const Student& b) const {
        return a.id < b.id; // Compare by ID
    }
};

// ============================ Generic AVL ================================
template <typename Key, typename Value, typename Compare = std::less<Key>>
class AVL {
public:
    struct Node {
        Key key;
        Value val;
        int left, right, parent, height;
        bool used;
        Node() : left(-1), right(-1), parent(-1), height(1), used(false) {}
    };

    Node* nodes;
    int root, freeList, n, capacity;
    Compare cmp;

public:
    AVL(int cap = 64) : root(-1), n(0), capacity(cap), cmp(Compare()) {
        nodes = new Node[capacity];
        for (int i = 0; i < capacity - 1; i++) nodes[i].parent = i + 1;
        nodes[capacity - 1].parent = -1;
        freeList = 0;
    }

    ~AVL() { delete[] nodes; }

private:
    // ====================== Utility ======================
    int height(int i) const { return (i == -1) ? 0 : nodes[i].height; }

    void updateHeight(int i) {
        nodes[i].height = 1 + max(height(nodes[i].left), height(nodes[i].right));
    }

    int balanceFactor(int i) const {
        return (i == -1) ? 0 : height(nodes[i].left) - height(nodes[i].right);
    }

    int newNode(const Key& k, const Value& v) {
        if (freeList == -1) throw runtime_error("Node overflow");
        int idx = freeList;
        freeList = nodes[idx].parent;
        nodes[idx] = Node();
        nodes[idx].key = k;
        nodes[idx].val = v;
        nodes[idx].used = true;
        n++;
        return idx;
    }

    void freeNode(int i) {
        nodes[i].used = false;
        nodes[i].parent = freeList;
        freeList = i;
        n--;
    }

    // ====================== Rotations ======================
    int rotateRight(int y) {
        int x = nodes[y].left;
        int T2 = nodes[x].right;

        nodes[x].right = y;
        nodes[y].left = T2;

        if (T2 != -1) nodes[T2].parent = y;
        nodes[x].parent = nodes[y].parent;
        nodes[y].parent = x;

        updateHeight(y);
        updateHeight(x);
        return x;
    }

    int rotateLeft(int x) {
        int y = nodes[x].right;
        int T2 = nodes[y].left;

        nodes[y].left = x;
        nodes[x].right = T2;

        if (T2 != -1) nodes[T2].parent = x;
        nodes[y].parent = nodes[x].parent;
        nodes[x].parent = y;

        updateHeight(x);
        updateHeight(y);
        return y;
    }

    // ====================== Insert ======================
    int insertRec(int cur, int parent, const Key& k, const Value& v) {
        if (cur == -1) {
            int idx = newNode(k, v);
            nodes[idx].parent = parent;
            return idx;
        }

        if (cmp(k, nodes[cur].key))
            nodes[cur].left = insertRec(nodes[cur].left, cur, k, v);
        else if (cmp(nodes[cur].key, k))
            nodes[cur].right = insertRec(nodes[cur].right, cur, k, v);
        else {
            nodes[cur].val = v;
            return cur;
        }

        updateHeight(cur);
        int bf = balanceFactor(cur);

        // Balancing
        if (bf > 1 && cmp(k, nodes[nodes[cur].left].key))
            return rotateRight(cur);
        if (bf < -1 && cmp(nodes[nodes[cur].right].key, k))
            return rotateLeft(cur);
        if (bf > 1 && cmp(nodes[nodes[cur].left].key, k) == false) {
            nodes[cur].left = rotateLeft(nodes[cur].left);
            return rotateRight(cur);
        }
        if (bf < -1 && cmp(k, nodes[nodes[cur].right].key)) {
            nodes[cur].right = rotateRight(nodes[cur].right);
            return rotateLeft(cur);
        }
        return cur;
    }

    // ====================== Deletion ======================
    int minNode(int cur) const {
        while (cur != -1 && nodes[cur].left != -1) cur = nodes[cur].left;
        return cur;
    }

    int removeRec(int cur, const Key& k) {
        if (cur == -1) return -1;

        if (cmp(k, nodes[cur].key))
            nodes[cur].left = removeRec(nodes[cur].left, k);
        else if (cmp(nodes[cur].key, k))
            nodes[cur].right = removeRec(nodes[cur].right, k);
        else {
            if (nodes[cur].left == -1 || nodes[cur].right == -1) {
                int child = (nodes[cur].left != -1) ? nodes[cur].left : nodes[cur].right;
                if (child != -1) nodes[child].parent = nodes[cur].parent;
                freeNode(cur);
                return child;
            } else {
                int succ = minNode(nodes[cur].right);
                nodes[cur].key = nodes[succ].key;
                nodes[cur].val = nodes[succ].val;
                nodes[cur].right = removeRec(nodes[cur].right, nodes[succ].key);
            }
        }

        updateHeight(cur);
        int bf = balanceFactor(cur);

        if (bf > 1 && balanceFactor(nodes[cur].left) >= 0)
            return rotateRight(cur);
        if (bf > 1 && balanceFactor(nodes[cur].left) < 0) {
            nodes[cur].left = rotateLeft(nodes[cur].left);
            return rotateRight(cur);
        }
        if (bf < -1 && balanceFactor(nodes[cur].right) <= 0)
            return rotateLeft(cur);
        if (bf < -1 && balanceFactor(nodes[cur].right) > 0) {
            nodes[cur].right = rotateRight(nodes[cur].right);
            return rotateLeft(cur);
        }

        return cur;
    }

    // ====================== Traversals ======================
    void inorderRec(int cur) const {
        if (cur == -1) return;
        inorderRec(nodes[cur].left);
        cout << nodes[cur].key << " : " << nodes[cur].val << " | ";
        inorderRec(nodes[cur].right);
    }

    void preorderRec(int cur) const {
        if (cur == -1) return;
        cout << nodes[cur].key << " : " << nodes[cur].val << " | ";
        preorderRec(nodes[cur].left);
        preorderRec(nodes[cur].right);
    }

    void postorderRec(int cur) const {
        if (cur == -1) return;
        postorderRec(nodes[cur].left);
        postorderRec(nodes[cur].right);
        cout << nodes[cur].key << " : " << nodes[cur].val << " | ";
    }

public:
    // ====================== Public APIs ======================
    void insert(const Key& k, const Value& v) { root = insertRec(root, -1, k, v); }

    void remove(const Key& k) { root = removeRec(root, k); }

    bool find(const Key& k, Value& out) const {
        int cur = root;
        while (cur != -1) {
            if (cmp(k, nodes[cur].key))
                cur = nodes[cur].left;
            else if (cmp(nodes[cur].key, k))
                cur = nodes[cur].right;
            else {
                out = nodes[cur].val;
                return true;
            }
        }
        return false;
    }

    void inorder() const { inorderRec(root); cout << endl; }
    void preorder() const { preorderRec(root); cout << endl; }
    void postorder() const { postorderRec(root); cout << endl; }

    void levelOrder() const {
        if (root == -1) return;
        queue<int> q;
        q.push(root);
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            cout << nodes[cur].key << ":" << nodes[cur].val << " ";
            if (nodes[cur].left != -1) q.push(nodes[cur].left);
            if (nodes[cur].right != -1) q.push(nodes[cur].right);
        }
        cout << endl;
    }

    int height() const { return height(root); }
    int size() const { return n; }

    void clear() {
        root = -1;
        n = 0;
        for (int i = 0; i < capacity - 1; i++) nodes[i].parent = i + 1;
        nodes[capacity - 1].parent = -1;
        freeList = 0;
    }

    // ====================== Advanced ======================
    int findIndex(const Key& k) const {
        int cur = root;
        while (cur != -1) {
            if (cmp(k, nodes[cur].key)) cur = nodes[cur].left;
            else if (cmp(nodes[cur].key, k)) cur = nodes[cur].right;
            else return cur;
        }
        return -1;
    }

    int predecessor(const Key& k) const {
        int pred = -1, cur = root;
        while (cur != -1) {
            if (cmp(nodes[cur].key, k)) {
                pred = cur;
                cur = nodes[cur].right;
            } else cur = nodes[cur].left;
        }
        return pred;
    }

    int successor(const Key& k) const {
        int succ = -1, cur = root;
        while (cur != -1) {
            if (cmp(k, nodes[cur].key)) {
                succ = cur;
                cur = nodes[cur].left;
            } else cur = nodes[cur].right;
        }
        return succ;
    }

    int LCA(const Key& a, const Key& b) const {
        int cur = root;
        while (cur != -1) {
            if (cmp(nodes[cur].key, a) && cmp(nodes[cur].key, b))
                cur = nodes[cur].right;
            else if (cmp(a, nodes[cur].key) && cmp(b, nodes[cur].key))
                cur = nodes[cur].left;
            else return cur;
        }
        return -1;
    }

    // kth smallest (1-indexed)
    int kthSmallest(int k) const {
        vector<int> inorderList;
        function<void(int)> collect = [&](int cur) {
            if (cur == -1) return;
            collect(nodes[cur].left);
            inorderList.push_back(cur);
            collect(nodes[cur].right);
        };
        collect(root);
        if (k < 1 || k > (int)inorderList.size()) return -1;
        return inorderList[k - 1];
    }

    bool isBST() const {
        return isBSTUtil(root, numeric_limits<Key>::lowest(), numeric_limits<Key>::max());
    }

    bool isBSTUtil(int cur, const Key& minK, const Key& maxK) const {
        if (cur == -1) return true;
        if (cmp(nodes[cur].key, minK) || cmp(maxK, nodes[cur].key)) return false;
        return isBSTUtil(nodes[cur].left, minK, nodes[cur].key) &&
               isBSTUtil(nodes[cur].right, nodes[cur].key, maxK);
    }
};

// ====================== DEMO ======================
int main() {
    AVL<Student, string, StudentCompare> t(50);

    t.insert({1, 8.5}, "Alice");
    t.insert({3, 9.2}, "Bob");
    t.insert({2, 8.9}, "Charlie");
    t.insert({4, 7.8}, "David");
    t.insert({5, 8.0}, "Eve");

    cout << "Inorder:\n"; t.inorder();
    cout << "Preorder:\n"; t.preorder();
    cout << "Level Order:\n"; t.levelOrder();

    cout << "\nHeight: " << t.height() << ", Size: " << t.size() << endl;

    string val;
    if (t.find({3, 0}, val)) cout << "Found key {3,_} → " << val << endl;

    int pred = t.predecessor({3, 0});
    if (pred != -1) cout << "Predecessor of {3,_} → " << t.nodes[pred].key << endl;

    int succ = t.successor({3, 0});
    if (succ != -1) cout << "Successor of {3,_} → " << t.nodes[succ].key << endl;

    int kth = t.kthSmallest(2);
    if (kth != -1) cout << "2nd Smallest → " << t.nodes[kth].key << endl;

    t.remove({2, 0});
    cout << "\nAfter deletion:\n"; t.inorder();
}
