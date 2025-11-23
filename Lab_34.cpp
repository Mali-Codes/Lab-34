#include <iostream>
#include <vector>
using namespace std;

const int SIZE = 9;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;
    vector<string> stationNames;

    // Graph Constructor
    Graph(vector<Edge> const &edges, vector<string> const &names) {
        adjList.resize(SIZE);
        stationNames = names;

        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            adjList[src].push_back(make_pair(dest, weight));
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print the network 
    void printNetwork() {
        cout << "City Transportation Network Topology:" << endl;
        cout << "======================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << "Station " << i << " (" << stationNames[i] << ") connects to:" << endl;
            for (Pair v : adjList[i])
                cout << "  → Station " << v.first << " (Travel Time: " << v.second << " min)" << endl;
            cout << endl;
        }
    }

    // DFS helper function using recursion
    void DFSUtil(int vertex, vector<bool> &visited) {
    visited[vertex] = true;  // "I'm at vertex now, mark it"
    cout << vertex << " ";
    
    // "Let me check who I can visit from here"
    for (auto &adjacent : adjList[vertex]) {  // Look at neighbors
        int adjVertex = adjacent.first;
        if (!visited[adjVertex]) {  // "Have I been there?"
            DFSUtil(adjVertex, visited);  // "Nope! Let's go there!"
            }
        }
    }

    void inspectNetwork(int startVertex) {
        vector<bool> visited(SIZE, false);

        cout << "Network Trace (DFS) from Station " << startVertex 
             << " (" << stationNames[startVertex] << "):" << endl;
        cout << "Purpose: Checking all reachable stations for maintenance" << endl;
        cout << "========================================" << endl;
        
        inspectNetworkUtil(startVertex, visited, "");
        cout << endl;
    }

    // DFS utility for network inspection with details
    void inspectNetworkUtil(int vertex, vector<bool> &visited, string indent) {
        visited[vertex] = true;
        
        cout << indent << "Inspecting Station " << vertex 
             << " (" << stationNames[vertex] << ")" << endl;
        
        for (auto &adjacent : adjList[vertex]) {
            int adjVertex = adjacent.first;
            int travelTime = adjacent.second;
            
            if (!visited[adjVertex]) {
                cout << indent << "  → Route to Station " << adjVertex 
                     << " (" << stationNames[adjVertex] 
                     << ") - Travel Time: " << travelTime << " min" << endl;
                inspectNetworkUtil(adjVertex, visited, indent + "  ");
            }
        }
    }

    // BFS for finding service coverage
    void analyzeCoverage(int startVertex) {
        vector<bool> visited(SIZE, false);
        vector<int> distance(SIZE, -1);
        queue<int> q;

        visited[startVertex] = true;
        distance[startVertex] = 0;
        q.push(startVertex);

        cout << "Service Coverage Analysis (BFS) from Station " << startVertex 
             << " (" << stationNames[startVertex] << "):" << endl;
        cout << "Purpose: Finding shortest routes to all stations" << endl;
        cout << "========================================" << endl;

        while (!q.empty()) {
            int vertex = q.front();
            q.pop();
            
            cout << "Checking Station " << vertex 
                 << " (" << stationNames[vertex] << ")" << endl;

            for (auto &adjacent : adjList[vertex]) {
                int adjVertex = adjacent.first;
                int travelTime = adjacent.second;
                
                if (!visited[adjVertex]) {
                    visited[adjVertex] = true;
                    distance[adjVertex] = distance[vertex] + 1;
                    q.push(adjVertex);
                    
                    cout << "  → Next stop: Station " << adjVertex 
                         << " (" << stationNames[adjVertex] 
                         << ") - Travel Time: " << travelTime << " min" << endl;
                }
            }
        }
        cout << endl;
    }

    void displayRouteInfo(int from, int to) {
        if (from >= SIZE || to >= SIZE) {
            cout << "Invalid station numbers!" << endl;
            return;
        }
        
        cout << "Route Information:" << endl;
        cout << "From: Station " << from << " (" << stationNames[from] << ")" << endl;
        cout << "To: Station " << to << " (" << stationNames[to] << ")" << endl;
        
        // Check if direct connection exists
        for (auto &adjacent : adjList[from]) {
            if (adjacent.first == to) {
                cout << "Direct route available - Travel Time: " 
                     << adjacent.second << " min" << endl;
                return;
            }
        }
        cout << "No direct route - Transfer required" << endl;
    }
};










int main() {
    // Station names for the transportation network
    vector<string> stationNames = {
        "Central Terminal",      // 0
        "Downtown Hub",          // 1
        "Airport Express",       // 2
        "University Station",    // 3
        "Shopping District",     // 4
        "Medical Center",        // 5
        "Sports Complex",        // 6
        "Business Park",         // 7
        "Residential Zone"       // 8
    };

    // Create transportation network edges (station pairs with travel times in minutes)
    vector<Edge> edges = {
        {0,1,8},{0,2,21},{1,2,6},{2,7,11},{2,8,8},
        {3,1,5},{3,4,9},{4,1,4},{4,3,9},
        {5,6,10},{5,7,15},{5,8,5},
        {6,5,10},{6,7,3},{6,8,7},
        {7,6,3},{7,5,15},{7,2,11},
        {8,5,5},{8,6,7},{8,2,8}
    };

    // Create the transportation network graph
    Graph network(edges, stationNames);

    // Display the network topology
    network.printNetwork();

    cout << endl;

    // Perform network inspection using DFS from Central Terminal
    network.inspectNetwork(0);

    cout << endl;

    // Analyze service coverage using BFS from Central Terminal
    network.analyzeCoverage(0);

    cout << endl;

    // Show some example route information
    cout << "Example Route Queries:" << endl;
    cout << "=====================" << endl;
    network.displayRouteInfo(0, 1);
    cout << endl;
    network.displayRouteInfo(3, 7);

    return 0;
}
