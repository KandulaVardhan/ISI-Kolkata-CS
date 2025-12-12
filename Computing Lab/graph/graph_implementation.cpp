#include <bits/stdc++.h>
#include "dsu.cpp"

using namespace std;
class Node{
    public:
    int start;
    int end;
    float weight;
};

class Graph {
public:
    int n, m;
    bool directed;
    bool weighted;

    vector<vector<int>> adj;
    vector<vector<float>> wt;

    vector<int> component_labels;
    int num_components = 0;

    Graph(bool directed_flag = false, bool weighted_flag = false) {
        directed = directed_flag;
        weighted = weighted_flag;
        n = m = 0;
    }

    /* ----------------------------------------------------
       READ GRAPH
       Supports 4 modes:
       (1) directed/unweighted
       (2) directed/weighted
       (3) undirected/unweighted
       (4) undirected/weighted

       Input format:
       n
       deg(u)  v1 [w1]  v2 [w2] ...
       ---------------------------------------------------- */
    bool readGraph(const string &filename) {
        ifstream in(filename);
        if (!in) {
            cerr << "Error opening file\n";
            return false;
        }

        in >> n;
        adj.assign(n, {});
        if (weighted) wt.assign(n, {});

        m = 0;

        for (int u = 0; u < n; u++) {
            int deg;
            in >> deg;
            for (int j = 0; j < deg; j++) {
                int v;
                float w = 1;

                if (weighted) {
                    
                    in >> v >> w;
                    cout<<v<<" "<<w<<endl;
                    adj[u].push_back(v);
                    wt[u].push_back(w);
                } else {
                    in >> v;
                    adj[u].push_back(v);
                }

                m++;
            }
            string str;
            in >> str;
        }

        component_labels.assign(n, 0);
        return true;
    }

    /* ----------------------------------------------------
       PRINT GRAPH
       ---------------------------------------------------- */
    void printGraph() const {
        cout << (directed ? "Directed" : "Undirected")
             << ", " << (weighted ? "Weighted" : "Unweighted") << "\n";

        cout << "Vertices: " << n << "\nEdges: " << m << "\n\n";
        
        for (int i = 0; i < n; i++) {
            cout << "v" << i << ": ";
            if (weighted) {
                for (int j = 0; j < adj[i].size(); j++)
                    cout << "(" << adj[i][j] << ", " << wt[i][j] << ") ";
            } else {
                for (int v : adj[i])
                    cout << v << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    /* ----------------------------------------------------
       DFS
       ---------------------------------------------------- */
    void dfsUtil(int u, vector<bool> &vis) const {
        vis[u] = true;
        cout << u << " ";
        for (int v : adj[u])
            if (!vis[v]) dfsUtil(v, vis);
    }

    void dfs(int start) const {
        vector<bool> vis(n, false);
        
        for(int i=0;i<n;i++){
            if(vis[i] == false){
                cout << "DFS: ";
                dfsUtil(i, vis);
                cout << "\n";
            }
            
        }
        
    }
    
    /* ----------------------------------------------------
       BFS
       ---------------------------------------------------- */

    
    void bfs(int start) const {
        vector<bool> vis(n, false);
        
        for(int i=0; i<n; i++){
            if(vis[i]==false){
                queue<int> q;

                q.push(i);
                vis[i] = true;

                cout << "BFS: ";
                while (!q.empty()) {
                    int u = q.front(); q.pop();
                    cout << u << " ";

                    for (int v : adj[u]) {
                        if (!vis[v]) {
                            vis[v] = true;
                            q.push(v);
                        }
                    }
                }
                cout << "\n";

            }
        }
    }

    

    /* ----------------------------------------------------
       CONNECTED COMPONENTS 
       (Only valid for UNDIRECTED)
       ---------------------------------------------------- */
    void dfsCC(int u, int cid) {
        component_labels[u] = cid;
        for (int v : adj[u])
            if (component_labels[v] == 0)
                dfsCC(v, cid);
    }

    int labelConnectedComponents() {
        if (directed) {
            cout << "Connected components only valid for undirected graph.\n";
            return -1;
        }

        num_components = 0;
        fill(component_labels.begin(), component_labels.end(), 0);

        for (int i = 0; i < n; i++) {
            if (component_labels[i] == 0) {
                num_components++;
                dfsCC(i, num_components);
            }
        }
        return num_components;
    }
    
    //cycle detection Undirected
    bool dfs_cycle_undirected(int start, int parent, vector<int> &vis) {
        vis[start] = 1;
        for(int v: adj[start]){
            if(!vis[v]){
                bool temp = dfs_cycle_undirected(v, start, vis );
                if(temp == true)
                return true;
            }
            else{
                if(v!=parent){
                    return true;
                }
            }
        }
        return false;
        
    }
   
    bool cycle_detection_undirected(){
        vector <int> vis(n,0);
        for(int i=0; i<n; i++){
            if(!vis[i]){
                bool ans = dfs_cycle_undirected(i, -1, vis);
                if(ans)
                return true;
            }
        }
        return false;
    }

    //cycle detection Directed
    bool dfs_cycle_directed(int node, vector<int> &vis, vector<int> &pathVis) {

        vis[node] = 1;       // mark visited
        pathVis[node] = 1;   // mark in current recursion stack

        for(int v : adj[node]) {

            // Case 1: If not visited, explore recursively
            if(!vis[v]) {
                if(dfs_cycle_directed(v, vis, pathVis))
                    return true;
            }

            // Case 2: If visited and it's in the recursion stack --> cycle
            else if(pathVis[v] == 1) {
                return true;
            }
        }

        pathVis[node] = 0;  // backtrack
        return false;
    }
    bool cycle_detection_directed() {
        vector<int> vis(n, 0);
        vector<int> pathVis(n, 0);

        for(int i=0; i<n; i++){
            if(!vis[i]){
                if(dfs_cycle_directed(i, vis, pathVis))
                    return true;
            }
        }
        return false;   
    }
    //adj list to edge list
    vector <Node*> edge_list_from_Adj_list(){
        vector <Node*> temp;
        for(int i=0; i<n; i++){
            for(int j=0; j<adj[i].size(); j++){
                Node *arr= new Node();
                arr->start = i;
                arr->end= adj[i][j];
                
                
                if(weighted){
                    arr->weight=wt[i][j];
                    temp.push_back(arr);
                }
                else{
                    arr->weight=0;
                    temp.push_back(arr);
                }

            }
        }
        sort(temp.begin(), temp.end(), [](const Node* x, const Node* y){
            return x->weight < y->weight;
        });
        return temp;
    }

    /* ----------------------------------------------------
       Dijkstra (weighted only)
       ---------------------------------------------------- */
    void dijkstra(int src) const {
        if (!weighted) {
            cout << "Dijkstra works only on weighted graphs.\n";
            return;
        }

        const float INF = 1e18;
        vector<float> dist(n, INF);
        vector<int> parent(n, -1);

        priority_queue<
            pair<float,int>,
            vector<pair<float,int>>,
            greater<pair<float,int>>
        > pq; //minheap

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto cd = pq.top().first;
            auto u = pq.top().second;

            pq.pop();

            if (cd != dist[u]) continue;

            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                float w = wt[u][i];

                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Dijkstra from " << src << ":\n";
        for (int i = 0; i < n; i++)
            cout << "v" << i << "  dist=" << dist[i]
                 << "  parent=" << parent[i] << "\n";
        cout << "\n";
    }

    /* ----------------------------------------------------
       Bellman-Ford (weighted only)
       ---------------------------------------------------- */
    void bellmanFord(int src) const {
        if (!weighted) {
            cout << "Bellman-Ford works only on weighted graphs.\n";
            return;
        }

        const float INF = 1e18;
        vector<float> dist(n, INF);
        vector<int> parent(n, -1);

        dist[src] = 0;

        // Relax edges V-1 times
        for (int k = 0; k < n - 1; k++) {
            for (int u = 0; u < n; u++) {
                for (int i = 0; i < adj[u].size(); i++) {
                    int v = adj[u][i];
                    float w = wt[u][i];

                    if (dist[u] < INF && dist[v] > dist[u] + w) {
                        dist[v] = dist[u] + w;
                        parent[v] = u;
                    }
                }
            }
        }

        // Check for negative cycle
        for (int u = 0; u < n; u++) {
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                float w = wt[u][i];

                if (dist[u] < INF && dist[v] > dist[u] + w) {
                    cout << "Negative cycle detected!\n";
                    return;
                }
            }
        }

        cout << "Bellman-Ford from " << src << ":\n";
        for (int i = 0; i < n; i++)
            cout << "v" << i << "  dist=" << dist[i]
                 << "  parent=" << parent[i] << "\n";
        cout << "\n";
    }

    // Kahn's Algo (Directed Acyclic Topological sort)
    void directed_acyclic_graph(){
        vector <int> hash_array(n,0);
        for(int i=0; i<n; i++){
            for(int v: adj[i]){
                hash_array[v]++;
            }

        }
        for(int i=0; i<n; i++){
            cout<<hash_array[i]<<" ";
        }

        
        queue <int> q;//minheap
        for(int i=0; i<hash_array.size(); i++){
            if(hash_array[i]==0){
                q.push(i);
            }
            
        }
        while(!q.empty()){
            int i=q.front();
            q.pop();
            
            for(int v:adj[i]){
                hash_array[v]--;
                if(hash_array[v]==0){
                    q.push(v);
                }
                
            }
            cout<<i<<"->";


        }
        return;
        
    }

    // 2-colorable graph / bipartite
    void colorable(){
        vector <char> arr(n,'k');
        arr[0]='R';
        queue <int> q;
        for(int v:adj[0]){
            q.push(v);
        }
        while(!q.empty()){
            int i=q.front();
            q.pop();
            for(int v:adj[i]){
                if(arr[i]=='k'){
                    if(arr[i]=='R'){
                        arr[v]='Y';
                    }
                    else{
                        arr[v]='R';
                    }
                    q.push(v);
                }
                else{
                    if(arr[i]==arr[v]){
                        cout<<"Not colorable";
                        return;
                    }

                }
            }
        }
        cout<<"Colorable";
        return;

    }

    // kruskal's MST
    void kruskal() {
        if (!weighted || directed) {
            cout << "Kruskal requires an undirected, weighted graph.\n";
            return;
        }

        struct Edge {
            int u, v;
            float w;
        };

        vector<Edge> edges;

        // collect edges (avoid duplicates because graph is undirected)
        for (int u = 0; u < n; u++) {
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                float w = wt[u][i];

                if (u < v) edges.push_back({u, v, w});
            }
        }

        sort(edges.begin(), edges.end(),
            [](const Edge &a, const Edge &b) {
                return a.w < b.w;
            });

        DSU dsu(n);
        float mst_cost = 0;

        cout << "Kruskal MST edges:\n";

        for (auto &e : edges) {
            if (!dsu.connected(e.u, e.v)) {
                dsu.unite(e.u, e.v);

                cout << e.u << " -- " << e.v << "  weight=" << e.w << "\n";

                mst_cost += e.w;
            }
        }

        cout << "Total MST Cost = " << mst_cost << "\n\n";
    }

    // prim's MST
    void prim(int start = 0) {
        if (!weighted || directed) {
            cout << "Prim requires an undirected, weighted graph.\n";
            return;
        }

        vector<float> key(n, 1e18);
        vector<int> parent(n, -1);
        vector<bool> inMST(n, false);

        key[start] = 0;

        // min-heap: (key, node)
        priority_queue<
            pair<float,int>,
            vector<pair<float,int>>,
            greater<pair<float,int>>
        > pq;

        pq.push({0, start});

        cout << "Prim MST edges:\n";

        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();

            int u = top.second;

            if (inMST[u]) continue;
            inMST[u] = true;

            if (parent[u] != -1)
                cout << parent[u] << " -- " << u << "  weight=" << key[u] << "\n";

            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                float w = wt[u][i];

                if (!inMST[v] && w < key[v]) {
                    key[v] = w;
                    parent[v] = u;
                    pq.push({key[v], v});
                }
            }
        }

        float cost = 0;
        for (int i = 0; i < n; i++)
            if (key[i] < 1e18) cost += key[i];

        cout << "Total MST Cost = " << cost << "\n\n";
    }

    // finding all pairs shortest paths
    void floydWarshall() const {
        if (!weighted) {
            cout << "Floyd–Warshall works only on weighted graphs.\n";
            return;
        }

        const float INF = 1e18;
        vector<vector<float>> dist(n, vector<float>(n, INF));

        for (int i = 0; i < n; i++)
            dist[i][i] = 0;

        for (int u = 0; u < n; u++)
            for (int j = 0; j < adj[u].size(); j++) {
                int v = adj[u][j];
                float w = wt[u][j];
                dist[u][v] = min(dist[u][v], w);
            }

        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];

        cout << "Floyd-Warshall All-Pairs Shortest Paths:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][j] >= INF/2) cout << "INF ";
                else cout << dist[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    // finding strongly connected components
    //
    vector<vector<int>> kosaraju(int n, const vector<vector<int>>& adj) {
        vector<vector<int>> radj(n);      // reversed graph
        vector<bool> visited(n, false);
        stack<int> st;
        vector<vector<int>> sccs;

        // ---- Step 1 DFS (store finishing order) ----
        function<void(int)> dfs1 = [&](int u) {
            visited[u] = true;
            for (int v : adj[u])
                if (!visited[v])
                    dfs1(v);
            st.push(u);
        };

        for (int i = 0; i < n; i++)
            if (!visited[i])
                dfs1(i);

        // ---- Step 2: Reverse the graph ----
        for (int u = 0; u < n; u++)
            for (int v : adj[u])
                radj[v].push_back(u);

        // ---- Step 3 DFS on reversed graph ----
        visited.assign(n, false);

        function<void(int, vector<int>&)> dfs2 = [&](int u, vector<int>& comp) {
            visited[u] = true;
            comp.push_back(u);
            for (int v : radj[u])
                if (!visited[v])
                    dfs2(v, comp);
        };

        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (!visited[u]) {
                vector<int> comp;
                dfs2(u, comp);
                sccs.push_back(comp);
            }
        }

        return sccs;
    }

    // Tarjan's algorithm of finding bridges
    vector<pair<int,int>> findBridges(int n, const vector<vector<int>>& adj) {
        vector<int> disc(n, -1), low(n, -1);
        vector<pair<int,int>> bridges;
        int timer = 0;

        function<void(int, int)> dfs = [&](int u, int parent) {
            disc[u] = low[u] = timer++;

            for (int v : adj[u]) {
                if (v == parent) continue;

                if (disc[v] == -1) {  
                    // Not visited
                    dfs(v, u);

                    low[u] = min(low[u], low[v]);

                    if (low[v] > disc[u]) {
                        bridges.push_back({u, v});
                    }
                }
                else {
                    // Back edge
                    low[u] = min(low[u], disc[v]);
                }
            }
        };

        for (int i = 0; i < n; i++)
            if (disc[i] == -1)
                dfs(i, -1);

        return bridges;
    }

    // finding articulation points
    vector<int> findArticulationPoints(int n, const vector<vector<int>>& adj) {
        vector<int> disc(n, -1), low(n, -1);
        vector<int> isAP(n, 0);
        int timer = 0;

        function<void(int, int)> dfs = [&](int u, int parent) {
            disc[u] = low[u] = timer++;
            int children = 0;

            for (int v : adj[u]) {

                if (v == parent) continue;

                if (disc[v] == -1) {  
                    children++;
                    dfs(v, u);

                    low[u] = min(low[u], low[v]);

                    // Case 1: u is root and has more than 1 child
                    if (parent == -1 && children > 1)
                        isAP[u] = 1;

                    // Case 2: u is not root, and v cannot reach above u
                    if (parent != -1 && low[v] >= disc[u])
                        isAP[u] = 1;

                } else {
                    // Back edge
                    low[u] = min(low[u], disc[v]);
                }
            }
        };

        for (int i = 0; i < n; i++)
            if (disc[i] == -1)
                dfs(i, -1);

        return isAP;   
        // isAP[i] = 1 if articulation point, else isAP[i] = 0
    }

    // deletes the vertex and re-numbers the adjacency list 
    void deleteAndCompress(int x) {
        if (x < 0 || x >= n) return;

        vector<int> mp(n, -1);
        int id = 0;

        for (int i = 0; i < n; i++) {
            if (i == x) continue;
            mp[i] = id++;
        }

        int newN = n - 1;

        vector<vector<int>> newAdj(newN);
        vector<vector<float>> newWt;
        if (weighted) newWt.assign(newN, vector<float>());

        for (int u = 0; u < n; u++) {
            if (u == x) continue;

            int nu = mp[u];

            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                if (v == x) continue;

                int nv = mp[v];
                newAdj[nu].push_back(nv);

                if (weighted)
                    newWt[nu].push_back(wt[u][i]);
            }
        }

        adj = newAdj;
        if (weighted) wt = newWt;
        n = newN;
    }

    // delete the edge between u,v and renumbers the vertices
    void deleteEdge(int u, int v) {
        if (u < 0 || u >= n || v < 0 || v >= n) return;

        // Remove v from adj[u]
        for (int i = 0; i < adj[u].size(); i++) {
            if (adj[u][i] == v) {
                adj[u].erase(adj[u].begin() + i);
                if (weighted)
                    wt[u].erase(wt[u].begin() + i);
                break;
            }
        }

        // If undirected, also remove u from adj[v]
        if (!directed) {
            for (int i = 0; i < adj[v].size(); i++) {
                if (adj[v][i] == u) {
                    adj[v].erase(adj[v].begin() + i);
                    if (weighted)
                        wt[v].erase(wt[v].begin() + i);
                    break;
                }
            }
        }

        m--;  
        if (!directed) m--;
    }

    // add edge between u,v
    void addEdge(int u, int v, float w = 1) {
        if (u < 0 || u >= n || v < 0 || v >= n) return;

        // Add (u → v)
        adj[u].push_back(v);
        if (weighted) wt[u].push_back(w);
        m++;

        // If undirected, also add (v → u)
        if (!directed) {
            adj[v].push_back(u);
            if (weighted) wt[v].push_back(w);
            m++;
        }
    }

};
