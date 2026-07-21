#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// ============================================================================
// Breadth-First Search (BFS)
// Best for: Shortest paths in unweighted graphs, level-by-level traversal.
// Time Complexity: O(V + E) | Space Complexity: O(V)
// ============================================================================
void bfs(int start, const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "BFS Traversal: ";
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        // Process the current node
        cout << u << " "; 

        // Traverse all adjacent nodes
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    cout << "\n";
}

// ============================================================================
// Depth-First Search (DFS)
// Best for: Topological sorting, cycle detection, strongly connected components.
// Time Complexity: O(V + E) | Space Complexity: O(V) (recursion stack)
// ============================================================================
void dfs_recursive(int u, const vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;
    
    // Process the current node
    cout << u << " ";

    // Recursively visit all unvisited neighbors
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs_recursive(v, adj, visited);
        }
    }
}

// Wrapper function to initialize the visited array and start DFS
void dfs(int start, const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    
    cout << "DFS Traversal: ";
    dfs_recursive(start, adj, visited);
    cout << "\n";
}

// ============================================================================
// Example Usage
// ============================================================================
int main() {
    // Optimize standard I/O operations for faster execution
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    // Example input: 5 vertices, 4 edges
    // Input: 5 4 \n 0 1 \n 0 2 \n 1 3 \n 2 4
    if (!(cin >> n >> m)) return 0;
    
    // Adjacency list representation
    // Size is n if nodes are 0-indexed. Use n+1 if nodes are 1-indexed.
    vector<vector<int>> adj(n); 
    
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        
        // For an undirected graph, add edges in both directions.
        // Remove the second line if the graph is directed.
        adj[u].push_back(v);
        adj[v].push_back(u); 
    }
    
    // Starting traversal from node 0
    bfs(0, adj);
    dfs(0, adj);
    
    return 0;
}
