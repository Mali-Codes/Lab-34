#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

const int SIZE = 9;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

class Graph {
public:
    vector<vector<Pair>> adjList;
    vector<string> nodeNames;

    // Graph Constructor
    Graph(vector<Edge> const &edges, vector<string> const &names) {
        adjList.resize(SIZE);
        nodeNames = names;

        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            adjList[src].push_back(make_pair(dest, weight));
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print the network topology
    void printNetwork() {
        cout << "Computer Network Topology:" << endl;
        cout << "======================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << "Node " << i << " (" << nodeNames[i] << ") connects to:" << endl;
            for (Pair v : adjList[i])
                cout << "  → Node " << v.first << " (Latency: " << v.second << " ms)" << endl;
        }
    }

    // DFS helper function
    void DFSUtil(int vertex, vector<bool> &visited) {
        visited[vertex] = true;
        cout << vertex << " ";

        for (auto &adjacent : adjList[vertex]) {
            int adjVertex = adjacent.first;
            if (!visited[adjVertex]) {
                DFSUtil(adjVertex, visited);
            }
        }
    }

    // Network Inspection using DFS
    void inspectNetwork(int startVertex) {
        vector<bool> visited(SIZE, false);

        cout << "Network Trace (DFS) from Node " << startVertex 
             << " (" << nodeNames[startVertex] << "):" << endl;
        cout << "Purpose: Tracing possible network paths for diagnostics" << endl;
        cout << "========================================" << endl;
        
        inspectNetworkUtil(startVertex, visited);
        cout << endl;
    }

    // DFS utility for network inspection with details
    void inspectNetworkUtil(int vertex, vector<bool> &visited) {
        visited[vertex] = true;
        
        cout << "Inspecting Node " << vertex 
             << " (" << nodeNames[vertex] << ")" << endl;
        
        for (auto &adjacent : adjList[vertex]) {
            int adjVertex = adjacent.first;
            int latency = adjacent.second;
            
            if (!visited[adjVertex]) {
                cout << "  → Connection to Node " << adjVertex 
                     << " (" << nodeNames[adjVertex] 
                     << ") - Latency: " << latency << " ms" << endl;
                inspectNetworkUtil(adjVertex, visited);
            }
        }
    }

    // BFS for analyzing network coverage
    void analyzeCoverage(int startVertex) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        cout << "Layer-by-Layer Network Inspection (BFS) from Node " << startVertex 
             << " (" << nodeNames[startVertex] << "):" << endl;
        cout << "Purpose: Analyzing network reachability by hop distance" << endl;
        cout << "========================================" << endl;

        while (!q.empty()) {
            int vertex = q.front();
            q.pop();
            
            cout << "Checking Node " << vertex 
                 << " (" << nodeNames[vertex] << ")" << endl;

            for (auto &adjacent : adjList[vertex]) {
                int adjVertex = adjacent.first;
                int latency = adjacent.second;
                
                if (!visited[adjVertex]) {
                    visited[adjVertex] = true;
                    q.push(adjVertex);
                    
                    cout << "  → Next hop: Node " << adjVertex 
                         << " (" << nodeNames[adjVertex] 
                         << ") - Latency: " << latency << " ms" << endl;
                }
            }
        }
    }

    // Dijkstra's algorithm to find shortest paths
    void dijkstra(int startVertex) {
        // Vector to store distances from start vertex
        vector<int> dist(SIZE, INT_MAX);
        
        // Priority queue to pick minimum distance vertex
        // Pair: (distance, vertex)
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        
        // Distance to start vertex is 0
        dist[startVertex] = 0;
        pq.push(make_pair(0, startVertex));
        
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            
            // Visit all adjacent vertices
            for (auto &adjacent : adjList[u]) {
                int v = adjacent.first;
                int weight = adjacent.second;
                
                // If there's a shorter path to v through u
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }
        
        // Print shortest paths
        cout << "Shortest path from node " << startVertex << ":" << endl;
        for (int i = 0; i < SIZE; i++) {
            cout << startVertex << " -> " << i << " : " << dist[i] << endl;
        }
    }

    void primMST() {
        // Vector to store the parent of each node in MST
        vector<int> parent(SIZE, -1);
        
        // Vector to store minimum weight edge to reach each node
        vector<int> key(SIZE, INT_MAX);
        
        // Vector to track nodes included in MST
        vector<bool> inMST(SIZE, false);
        
        // Priority queue: (weight, vertex)
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        
        // Start from vertex 0
        key[0] = 0;
        pq.push(make_pair(0, 0));
        
        int totalCost = 0;
        
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            
            // Skip if already in MST
            if (inMST[u]) continue;
            
            // Include vertex in MST
            inMST[u] = true;
            totalCost += key[u];
            
            // Check all adjacent vertices
            for (auto &adjacent : adjList[u]) {
                int v = adjacent.first;
                int weight = adjacent.second;
                
                // If v is not in MST and weight is smaller than current key
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    pq.push(make_pair(key[v], v));
                }
            }
        }
        
        // Print the MST
        cout << "Minimum Spanning Tree edges:" << endl;
        
        for (int i = 1; i < SIZE; i++) {
            if (parent[i] != -1) {
                cout << "Edge from " << parent[i] << " to " << i 
                     << " with capacity: " << key[i] << " units" << endl;
            }
        }
    }

};

int main() {
    // Node names for the computer network
    vector<string> nodeNames = {
        "Main Server",           // 0
        "Router A",              // 1
        "Router B",              // 2
        "Database Server",       // 3
        "Backup Server",         // 4
        "Load Balancer",         // 5
        "Firewall",              // 6
        "Cache Server",          // 7
        "Edge Server"            // 8
    };

    // Create computer network edges (node pairs with latency in milliseconds)
    vector<Edge> edges = {
        {0,1,8},{0,2,21},{1,2,6},{2,7,11},{2,8,8},
        {3,1,5},{3,4,9},{4,1,4},{4,3,9},
        {5,6,10},{5,7,15},{5,8,5},
        {6,5,10},{6,7,3},{6,8,7},
        {7,6,3},{7,5,15},{7,2,11},
        {8,5,5},{8,6,7},{8,2,8}
    };

    // Create the computer network graph
    Graph network(edges, nodeNames);

    // Display the network topology
    network.printNetwork();

    cout << endl;

    // Perform network inspection using DFS from Main Server
    network.inspectNetwork(0);

    cout << endl;

    // Analyze network coverage using BFS from Main Server
    network.analyzeCoverage(0);

    cout << endl;

    // Find shortest paths using Dijkstra's algorithm from Main Server
    network.dijkstra(0);

    cout << endl;

    // Find Minimum Spanning Tree using Prim's algorithm
    network.primMST();

    cout << endl;
    
    return 0;
}

// Step 4 complete