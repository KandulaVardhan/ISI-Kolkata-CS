#include <bits/stdc++.h>
using namespace std;
class Node{
    public:
        int data;
        Node * left;
        Node * right;
        Node (int x){
            data=x;
            left=NULL;
            right=NULL;
        }

};


class Compare{
    public:
        bool operator()(Node *a, Node *b){
            return a->data > b->data; // minheap
        }

};
int main(){
    Node * a= new Node(50);
    Node *b = new Node(8);
    priority_queue <Node*, vector <Node*>, Compare > pq;
    pq.push(a);
    pq.push(b);
    cout<<pq.top()->data;

}