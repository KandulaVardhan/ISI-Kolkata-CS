#include <bits/stdc++.h>
using namespace std;
int main(){
    int k;
    cin>>k;
    vector < queue <int>> arr(k);
    for(int i=0; i<k ;i++){
        int n;
        cin>>n;
        queue <int> temp;
        for(int j=0; j<n; j++){
            int ele;
            cin>>ele;
            temp.push(ele);
        }
        arr[i]=temp;
    }
    
    vector <int> answer;
    priority_queue<int,vector<int>,greater<int>>pq;
    unordered_map <int, int> m;
    for(int i=0; i<k; i++){
        pq.push(arr[i].front());
        m[arr[i].front()]=i;
    }
    // while(pq.empty()==false){
    //     int ele=pq.top();
    //     pq.pop();
    //     answer.push_back(ele);
    //     arr[m[ele]].pop();
    //     if(arr[m[ele]].empty() == false){
    //         int temp = arr[m[ele]].front();
    //         pq.push(temp);
    //         m[temp]=m[ele];
    //         m.erase(ele);
    //     }
    // }
    while (!pq.empty()) {
        int ele = pq.top();
        pq.pop();
        int idx = m[ele];
        m.erase(ele);  // erase the current element’s mapping

        answer.push_back(ele);
        arr[idx].pop();

        if (!arr[idx].empty()) {  // if that queue still has elements
            int temp = arr[idx].front();
            pq.push(temp);
            m[temp] = idx;
        }
    }
    for( int i=0; i<answer.size(); i++){
        cout<<answer[i]<<" ";
    }
    return 0;


    
}