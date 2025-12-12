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


bool path_root(TREE *t, int idx,   int data1, stack <int> s , stack <int> &ans){
    if(idx==-1){
        return false;
    }
    int curr=t->nodes[idx].data;
    s.push(curr);
    if(curr==data1){
        ans=s;
        return true;
    }
    else{
        bool l=path_root(t, t->nodes[idx].left, data1, s, ans);
        bool r=path_root(t, t->nodes[idx].right, data1, s, ans);
        return (l || r);

    }

}


int ancestor(TREE *t, int data1, int data2){
    stack <int> q1;
    stack<int> q2;
    stack <int> ans1;
    stack <int> ans2;
    bool d1=path_root(t, 0, data1, q1, ans1);
    bool d2=path_root(t, 0, data2, q2, ans2);
    // while(ans1.empty()==false){
    //     int ele=ans1.top();
    //     ans1.pop();
    //     cout<<ele<<" ";

    // }
    // cout<<endl;
    // while(ans2.empty()==false){
    //     int ele=ans2.top();
    //     ans2.pop();
    //     cout<<ele<<" ";
    // }
    
    if(ans1.size()>ans2.size()){
        while(ans1.size()!=ans2.size()){
            ans1.pop();
        }
    }
    else if(ans1.size()<ans2.size()){
        while(ans2.size()!=ans1.size()){
            ans2.pop();
        }
    }
    
    while(ans1.empty()==false){
        int a1=ans1.top();
        int a2=ans2.top();
        ans1.pop();
        ans2.pop();
        if(a1==a2){
            return a1;
        }

    }
    return -1;
}

int max_sum=INT_MIN;
int path_sum_max(TREE *t, int idx){
    if(idx== -1){
        return 0;
    }
    else if(t->nodes[idx].left==-1 && t->nodes[idx].right==-1){
        return t->nodes[idx].data;
    }
    else{
        int l=path_sum_max(t, t->nodes[idx].left);
        int r=path_sum_max(t,t->nodes[idx].right);
        int ele= t->nodes[idx].data +l+r  ;
        max_sum=max(ele, max_sum);
        return max(t->nodes[idx].data + l , t->nodes[idx].data + r);

    }
    return 0;
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
    // }
    // queue <int> q;
    // bool result=target_sum(&t, 0, 0, 16, q);
    // while(ans.empty()==false){
    //         int ele=ans.front();
    //         ans.pop();
    //         cout<<ele<<" ";
    // }
    
    int result=path_sum_max(&t, 0);
    cout<<max_sum;
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
