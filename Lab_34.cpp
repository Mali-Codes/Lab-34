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

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
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
    void DFS(int startVertex) {
        // Create a visited array to track visited vertices
        vector<bool> visited(SIZE, false);

        cout << "DFS starting from vertex " << startVertex << ":" << endl;
        DFSUtil(startVertex, visited);
        cout << endl;
    }


    void BFS(int startVertex) {
        // Create a visited array to track visited vertices
        vector<bool> visited(SIZE, false);

        // Create a queue for BFS
        queue<int> q;

        // Mark the start vertex as visited and enqueue it
        visited[startVertex] = true;
        q.push(startVertex);

        cout << "BFS starting from vertex " << startVertex << ":" << endl;

        while (!q.empty()) {
            // Dequeue a vertex and print it
            int vertex = q.front();
            q.pop();
            cout << vertex << " ";

            // Get all adjacent vertices of the dequeued vertex
            // If an adjacent vertex has not been visited, mark it visited and enqueue it
            for (auto &adjacent : adjList[vertex]) {
                int adjVertex = adjacent.first;
                if (!visited[adjVertex]) {
                    visited[adjVertex] = true;
                    q.push(adjVertex);
                }
            }
        }
        cout << endl;
    }    
};









int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,8},{0,2,21},{1,2,6},{2,7,11},{2,8,8},
        {3,1,5},{3,4,9},{4,1,4},{4,3,9},
        {5,6,10},{5,7,15},{5,8,5},
        {6,5,10},{6,7,3},{6,8,7},
        {7,6,3},{7,5,15},{7,2,11},
        {8,5,5},{8,6,7},{8,2,8}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();

    cout << endl;

    graph.DFS(0); // start from vertex 0
    
    graph.BFS(0); // start from vertx 0 again

    return 0;
}
