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
    vector<string> junctionNames;

    // Graph Constructor
    Graph(vector<Edge> const &edges, vector<string> const &names) {
        adjList.resize(SIZE);
        junctionNames = names;

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
        cout << "Water Pipeline Network Topology:" << endl;
        cout << "======================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << "Junction " << i << " (" << junctionNames[i] << ") connects to:" << endl;
            for (Pair v : adjList[i])
                cout << "  → Junction " << v.first << " (Capacity: " << v.second << " GPM)" << endl;
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

        cout << "Network Trace (DFS) from Junction " << startVertex 
             << " (" << junctionNames[startVertex] << "):" << endl;
        cout << "Purpose: Tracing possible contamination paths through the network" << endl;
        cout << "========================================" << endl;
        
        inspectNetworkUtil(startVertex, visited);
        cout << endl;
    }

    // DFS utility for network inspection with details
    void inspectNetworkUtil(int vertex, vector<bool> &visited) {
        visited[vertex] = true;
        
        cout << "Inspecting Junction " << vertex 
             << " (" << junctionNames[vertex] << ")" << endl;
        
        for (auto &adjacent : adjList[vertex]) {
            int adjVertex = adjacent.first;
            int capacity = adjacent.second;
            
            if (!visited[adjVertex]) {
                cout << "  → Potential spread to Junction " << adjVertex 
                     << " (" << junctionNames[adjVertex] 
                     << ") - Capacity: " << capacity << " GPM" << endl;
                inspectNetworkUtil(adjVertex, visited);
            }
        }
    }

    // BFS for analyzing service areas
    void analyzeCoverage(int startVertex) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        cout << "Layer-by-Layer Network Inspection (BFS) from Junction " << startVertex 
             << " (" << junctionNames[startVertex] << "):" << endl;
        cout << "Purpose: Analyzing service areas by distance from source" << endl;
        cout << "========================================" << endl;

        while (!q.empty()) {
            int vertex = q.front();
            q.pop();
            
            cout << "Checking Junction " << vertex 
                 << " (" << junctionNames[vertex] << ")" << endl;

            for (auto &adjacent : adjList[vertex]) {
                int adjVertex = adjacent.first;
                int capacity = adjacent.second;
                
                if (!visited[adjVertex]) {
                    visited[adjVertex] = true;
                    q.push(adjVertex);
                    
                    cout << "  → Next service area: Junction " << adjVertex 
                         << " (" << junctionNames[adjVertex] 
                         << ") - Capacity: " << capacity << " GPM" << endl;
                }
            }
        }
    }
};

int main() {
    // Junction names for the water pipeline network
    vector<string> junctionNames = {
        "Pump Station",          // 0
        "Distribution Hub",      // 1
        "Reservoir",             // 2
        "Treatment Plant",       // 3
        "Storage Tank",          // 4
        "Distribution Hub",      // 5
        "Control Valve",         // 6
        "Storage Tank",          // 7
        "Pump Station"           // 8
    };

    // Create water pipeline network edges (junction pairs with capacity in GPM)
    vector<Edge> edges = {
        {0,1,800},{0,2,2100},{1,2,600},{2,7,1100},{2,8,800},
        {3,1,500},{3,4,900},{4,1,400},{4,3,900},
        {5,6,1000},{5,7,1500},{5,8,500},
        {6,5,1000},{6,7,300},{6,8,700},
        {7,6,300},{7,5,1500},{7,2,1100},
        {8,5,500},{8,6,700},{8,2,800}
    };

    // Create the water pipeline network graph
    Graph network(edges, junctionNames);

    // Display the network topology
    network.printNetwork();

    cout << endl;

    // Perform network inspection using DFS from Junction 0 (Pump Station)
    network.inspectNetwork(0);

    cout << endl;

    // Analyze service coverage using BFS from Junction 0 (Pump Station)
    network.analyzeCoverage(0);

    return 0;
}

