#include <bits/stdc++.h>
using namespace std;

class DSU {
public:
    vector<int> parent, size;

    // Constructor
    DSU(int n) {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    // Find with path compression
    int find_parent(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find_parent(parent[x]);
    }

    // Union by size
    void unite(int a, int b) {
        a = find_parent(a);
        b = find_parent(b);
        if (a == b) return;

        // weighted union - find larger root
        if (size[a] < size[b])
            swap(a, b);

        parent[b] = a;
        size[a] += size[b];

    }

    // Check if two nodes are connected
    bool connected(int a, int b) {
        return find_parent(a) == find_parent(b);
    }

    // Get the size of the component containing node x
    int get_size(int x) {
        return size[find_parent(x)];
    }
};

