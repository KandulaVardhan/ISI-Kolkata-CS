#include <bits/stdc++.h>
#include "dsu.cpp"
using namespace std;
class weighted_edge{
    public:
        int u;
        int v;
        int w;
        
};

class compare{
    public:
        bool operator()( weighted_edge* a,  weighted_edge* b){
                if(a->w > b->w){ //min heap
                    return true;
                }
                else{
                    return false;
                }
            }

};

void cycle_detection(){
    int v,e;
    cin>>v>>e;
    DSU dsu(v);
    int flag=0;
    while(e--){
        int a,b;
        cin>>a>>b;
        
        if (dsu.connected(a,b)==true){
            flag=1;
            break;

        }
        else{
            dsu.unite(a,b);
        }
    }

    if(flag==1){
        cout<<"Graph has cycle";

    }
    else{
        cout<<"graph doesnt have cycle";
    }
}

void minimum_spanning_tree(){
    int v, e;
    cin>>v>>e;
    DSU dsu(v);
    
    priority_queue <weighted_edge *, vector <weighted_edge *>, compare > pq;
    while(e--){
        int a,b,c;
        cin>>a>>b>>c;
        weighted_edge * temp = new weighted_edge();
        temp->u=a;
        temp->v=b;
        temp->w=c;
        pq.push(temp);
    }
    int weight=0;
    int arr[v][v]={0};
    while(pq.empty()==false){
        weighted_edge * temp= pq.top();
        pq.pop();
        if(dsu.connected(temp->u, temp->v)){
            arr[temp->u][temp->v]=0;
            arr[temp->v][temp->u]=0;
            continue;
            
        }
        else{
            
            weight+=temp->w;
            dsu.unite(temp->u, temp->v);
            arr[temp->u][temp->v]=1;
            arr[temp->v][temp->u]=1;

        }

        

    }
    cout<<weight<<endl;
    for(int i=0; i< v; i++){
        for(int j=0; j< v; j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
}
// ======================= MAIN FUNCTION ============================
int main() {
    int n;
    cin>>n;
    DSU dsu(n+1);
    priority_queue <weighted_edge *, vector <weighted_edge *>, compare > pq;
    for(int i=1; i<=n; i++){
        weighted_edge * temp = new weighted_edge();
        int wt;
        cin>>wt;
        temp->u=i;
        temp->v=0;
        temp->w=wt;
        pq.push(temp);
    }

    for(int i=0; i<4; i++){
        weighted_edge * temp = new weighted_edge();
        int a,b,c;
        cin>>a;
        cin>>b;
        cin>>c;
        temp->u=a;
        temp->v=b;
        temp->w=c;
        pq.push(temp);
    }
    int weight=0;
    int arr[n][n]={0};
    while(pq.empty()==false){
        weighted_edge * temp= pq.top();
        pq.pop();
        if(dsu.connected(temp->u, temp->v)){
            arr[temp->u][temp->v]=0;
            arr[temp->v][temp->u]=0;
            continue;
            
        }
        else{
            
            weight+=temp->w;
            dsu.unite(temp->u, temp->v);
            arr[temp->u][temp->v]=1;
            arr[temp->v][temp->u]=1;

        }

        

    }
    cout<<"weight"<<weight<<endl;
    for(int i=0; i< n; i++){
        for(int j=0; j< n; j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }

    
    // cout << "Find(3): " << dsu.find_parent(3) << endl;
    // cout << "Find(7): " << dsu.find_parent(7) << endl;
    // cout << "Same(3, 7)? " << dsu.connected(3, 7) << endl;

    // dsu.unite(3, 7);
    // cout << "Same(3, 7)? " << dsu.connected(3, 7) << endl;

    // return 0;
}
