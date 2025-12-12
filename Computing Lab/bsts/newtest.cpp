#include"bst.cpp"
#include<bits/stdc++.h>
using namespace std;
// ===== Example Usage =====
int main() {
    BST t(15);

    cout << "Enter number of nodes and their (data left right):\n";
    // Example input:
    // 7
    // 50 1 2
    // 30 3 4
    // 70 5 6
    // 20 -1 -1
    // 40 -1 -1
    // 60 -1 -1
    // 80 -1 -1
    t.readTree();

    cout << "\nInorder traversal: ";
    t.printInorder();
    cout << "Height: " << t.getHeight() << ", Size: " << t.getSize() << "\n";
    cout << "Min: " << t.getMin() << ", Max: " << t.getMax() << "\n";
    cout << "Leaf Count: " << t.getLeafCount() << "\n";
    cout << "Is BST? " << (t.isBST() ? "Yes" : "No") << "\n";

    int a = t.findIndex(30), b = t.findIndex(40);
    cout << "Is 30 ancestor of 40? " << (t.isAncestor(a, b) ? "Yes" : "No") << "\n";
    cout << "LCA of 20 and 40 = " << t.findLCA(20, 40) << "\n";

    cout << "\nClearing tree...\n";
    t.clear();
    cout << "Tree cleared.\n";
}
