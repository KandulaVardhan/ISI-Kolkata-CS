#include <bits/stdc++.h>
using namespace std;

class ComparePair{
    public:
        bool operator()(const pair<int, int> &a, const pair<int, int> &b){
            return a.first < b.first; // maxheap
        }

};
int main() {
    priority_queue <pair<int, int> , vector<pair<int, int>>, ComparePair> pq;
    pq.push({1, 2});
    pq.push({4, 6});

    cout<<pq.top().first<<endl; // prints 4
    return 0;
}
