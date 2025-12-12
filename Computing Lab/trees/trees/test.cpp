#include <bits/stdc++.h>
using namespace std;

extern "C" {
#include "tree.h"
#include "tree_implementation.c"
}

// -------------------------
// New C++ STL function

// -------------------------

queue <int> ans;

void leaf_nonleaf_solve(TREE *t, vector <int> &non_leaf, vector <int> &leaf){
    if(t->root ==-1){
        cout<<"Tree DNE"<<endl;
        return;
    }
    queue <int> q;
    q.push(t->root);
    while(q.empty()==false){
        int idx=q.front();
        q.pop();
        if((t->nodes[idx].left == -1) && (t->nodes[idx].right==-1)){
            leaf.push_back(t->nodes[idx].data);
        }
        else{
            non_leaf.push_back(t->nodes[idx].data);
            if(t->nodes[idx].left!= -1){
                q.push(t->nodes[idx].left);
            }
            if(t->nodes[idx].right!= -1){
                q.push(t->nodes[idx].right);
            }
        }
        
    }
    return;

}

bool target_sum(TREE *t, int idx, int curr_sum,  int target, queue<int> q ){
    if(idx==-1){
        return false;
    }
    curr_sum += t->nodes[idx].data;
    q.push(t->nodes[idx].data);
    if(t->nodes[idx].left==-1 && t->nodes[idx].right==-1){
        if(curr_sum==target){
            ans=q;

            return true;
        }
        else{
            return false;
        }
    }
    else{
        bool l=target_sum(t, t->nodes[idx].left, curr_sum, target, q);
        bool r=target_sum(t, t->nodes[idx].right, curr_sum, target, q);
        return (l || r);

    }

}


int height(TREE *t, int idx){
    if(idx==-1){
        return 0;
    }
    return 1+max(height(t,t->nodes[idx].left) , height(t, t->nodes[idx].right) );
}

int min_depth(TREE *t, int idx){
    if(idx==-1){
        return 0;
    }
    if(t->nodes[idx].left != -1 && t->nodes[idx].right != -1){
        return 1+ min(min_depth(t, t->nodes[idx].left), min_depth(t, t->nodes[idx].right));
    }
    else{
        return 1;
    }

}



int main(){
    TREE t;
    init_tree(&t, 100);
    read_tree(&t);
    // int idx = find_index(&t, 60);
    // cout<<tree_insert(&t, 70, idx, 1)<<endl;

    //inorder(&t, 0);
    // vector <int> non_leaf;
    // vector <int> leaf;
    // leaf_nonleaf_solve(&t, non_leaf, leaf);
    // cout<<"NL:";
    // for(int i=0; i<non_leaf.size(); i++){
    //     cout<<non_leaf[i]<<" ";
    // }
    // cout<<endl;
    // for(int i=0; i<leaf.size(); i++){
    //     cout<<leaf[i]<<" ";
    // // }
    // queue <int> q;
    // bool result=target_sum(&t, 0, 0, 16, q);
    // while(ans.empty()==false){
    //         int ele=ans.front();
    //         ans.pop();
    //         cout<<ele<<" ";
    // }
    int h=min_depth(&t, t.root);
    cout<<h-1;
}

























// int main() {
//     TREE t;
//     read_tree(&t);

//     cout << "Inorder Traversal: ";
//     inorder(&t, t.root);

//     // cout << "\n";

//     // // Call our new C++ STL function
    

//     free_tree(&t);
//     return 0;
// }
