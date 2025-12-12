#include <bits/stdc++.h>
using namespace std;

#include "graph_implementation.cpp"   // include your updated Graph class

// defining a 2-d vector
// vector < vector <int > > dp(rows, vector<int> (columns, -1));

int main(int argc, char *argv[]) {

    bool directed = false;
    bool weighted = false;

    // -----------------------------
    // Parse command line arguments
    // -----------------------------
    vector<string> args;
    for (int i = 1; i < argc; i++) args.push_back(argv[i]);

    string filename = "";

    for (string s : args) {
        if (s == "-d") directed = true;
        else if (s == "-w") weighted = true;
        else filename = s;
    }

    if (filename == "") {
        cerr << "Usage: ./a.out [-d] [-w] <filename>\n";
        return 1;
    }

    // -----------------------------
    // Create Graph
    // -----------------------------
    Graph g(directed, weighted);

    if (!g.readGraph(filename)) {
        cerr << "Failed to read graph.\n";
        return 1;
    }

    // -----------------------------
    // Print graph
    // -----------------------------
    g.printGraph();

    // vector<Node*> temp = g.edge_list_from_Adj_list();
    // for(int i=0;i<temp.size();i++){
    //     cout<<temp[i]->start<<" "<<temp[i]->end<<" "<<temp[i]->weight<<endl;
    // }

    // if(!g.cycle_detection_directed()){
    //     g.directed_acyclic_graph();
    // }
    
    // if(!g.cycle_detection_undirected()){
    //     cout<<"Not colrable";
    // }
    // else{
    //     g.colorable();
    // }
    // if(g.cycle_detection_directed()){
    //     cout<<"cycle";
    // }
    // else{
    //     cout<<"no ";
    // }

    // // -----------------------------
    // // Traverse
    // // -----------------------------
    // cout << "Running DFS from node 0:\n";
    // g.dfs(0);

    // cout << "Running BFS from node 0:\n";
    // g.bfs(0);

    // // -----------------------------
    // // Connected Components
    // // -----------------------------
    // if (!directed) {
    //     cout << "\nConnected Components:\n";
    //     int cc = g.labelConnectedComponents();
    //     cout << "Number of components = " << cc << "\n";
    //     for (int i = 0; i < g.n; i++)
    //         cout << "v" << i << " → C" << g.component_labels[i] << "\n";
    // }

    // // -----------------------------
    // // Shortest Paths
    // // -----------------------------
    if (weighted) {
        cout << "\nDijkstra from node 0:\n";
        g.dijkstra(0);

        cout << "Bellman-Ford from node 0:\n";
        g.bellmanFord(0);
    }

    return 0;
}
