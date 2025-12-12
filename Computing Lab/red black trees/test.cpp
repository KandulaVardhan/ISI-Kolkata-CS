#include"llrb.cpp"
#include<bits/stdc++.h>
using namespace std;
int main() {
    LLRBTree<int, int> tree;
    tree.insert(5, 50);
    tree.insert(3, 30);
    tree.insert(7, 70);
    tree.insert(2, 20);
    tree.insert(4, 40);
    tree.insert(6, 60);
    tree.insert(8, 80);

    cout << "Inorder traversal: ";
    tree.inorderPrint();
}
