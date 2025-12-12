#include <bits/stdc++.h>
using namespace std;
#include "avl-implementation.cpp" 

// Try a few problems like:

// Count inversions in array using AVL.

// Find k-th smallest/largest using AVL.

// Range queries (count of keys in [L, R]).

// // ---------------- Demo main ----------------
int main() {

    AVLTree t(16);

    // Demo insertion
    vector<int> vals = {133, 1488, 145, 2316, 816, 402, 101, 80, 128, 1527};
    for (int v : vals) t.insert(-1, &t.root, v);

    // cout << "Inorder (recursive): ";
    // t.inorder(t.root);
    cout << "\n\nTree (sideways):\n";
    t.print_tree(t.root);

    // cout << "\nsize(root) = " << t.size(t.root) << "\n";
    // cout << "floor(26) = ";
    // int f = t.floor_key(t.root, 26);
    // if (f==INT_MIN) cout<<"(none)\n"; else cout<<f<<"\n";
    // cout << "ceil(26) = ";
    // int c = t.ceil_key(t.root, 26);
    // if (c==INT_MAX) cout<<"(none)\n"; else cout<<c<<"\n";
    // cout << "rank(26) = " << t.rank_of(t.root, 26) << "\n";

    // // Demo inversion counting (example): count inversions while inserting in the order arr
    // vector<int> arr = {8, 4, 2, 1}; // expects 6
    // AVLTree t2(16);
    // long long inv = 0;
    // for (int x : arr) {
    //     t2.insert_and_count(-1, &t2.root, x, inv);
    // }
    // cout << "\nInversions for {8,4,2,1} = " << inv << " (expected 6)\n";

    // // Demo delete
    // cout << "\nDeleting 20 and 30...\n";
    // t.remove_node(-1, &t.root, 20);
    // t.remove_node(-1, &t.root, 30);
    // cout << "Inorder after deletes: ";
    // t.inorder(t.root);
    // cout << "\n\nTree after deletes:\n";
    // t.print_tree(t.root);

    return 0;
}
