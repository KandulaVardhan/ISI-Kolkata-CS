#include<bits/stdc++.h>
#include "dsu.cpp"
using namespace std;
int main(){
    DSU dsu(4);
    cout<<"write edges"<<endl;
    while(1){
        int a, b;
        cin>>a>>b;
        if(dsu.find_parent(a) == dsu.find_parent(b)){
            cout<<"cycle formed"<<endl;
            break;
        }
        else{
            dsu.unite(a, b);
            cout<<"joined a, b"<<endl;
        }
    }
}