#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;
class Graph {
int V; // number of vertices
vector<vector<int>> adj; // adjacency list
public:
Graph(int V) {
this->V = V;
adj.resize(V);
}
void addEdge(int u, int v) {
adj[u].push_back(v);
adj[v].push_back(u); // undirected graph
}
// Parallel BFS
void parallelBFS(int start) {
vector<int> visited(V, 0);
queue<int> q;
visited[start] = 1;
q.push(start);
cout << "\nParallel BFS: ";
while (!q.empty()) {
int size = q.size();
vector<int> currentLevel;
for (int i = 0; i < size; i++) {
int node = q.front();
q.pop();
currentLevel.push_back(node);
}
#pragma omp parallel for
for (int i = 0; i < currentLevel.size(); i++) {
int node = currentLevel[i];
#pragma omp critical
cout << node << " ";
for (int neighbor : adj[node]) {
bool shouldAdd = false;
#pragma omp critical
{
if (!visited[neighbor]) {
visited[neighbor] = 1;
shouldAdd = true;
}
}
if (shouldAdd) {
#pragma omp critical
q.push(neighbor);
}
}
}
}
cout << endl;
}
// Parallel DFS helper
void parallelDFSUtil(int node, vector<int> &visited) {
#pragma omp critical
cout << node << " ";
for (int neighbor : adj[node]) {
bool shouldVisit = false;
#pragma omp critical
{
if (!visited[neighbor]) {
visited[neighbor] = 1;
shouldVisit = true;
}
}
if (shouldVisit) {
#pragma omp task
parallelDFSUtil(neighbor, visited);
}
}
}
// Parallel DFS
void parallelDFS(int start) {
vector<int> visited(V, 0);
cout << "\nParallel DFS: ";
#pragma omp parallel
{
#pragma omp single
{
visited[start] = 1;
parallelDFSUtil(start, visited);
}
}
cout << endl;
}
};
int main() {
int V, E;
cout << "Enter number of vertices: ";
cin >> V;
cout << "Enter number of edges: ";
cin >> E;
Graph g(V);
cout << "Enter edges (u v) zero-indexed:\n";
for (int i = 0; i < E; i++) {
int u, v;
cin >> u >> v;
g.addEdge(u, v);
}
int start;
cout << "Enter starting vertex: ";
cin >> start;
g.parallelBFS(start);
g.parallelDFS(start);
return 0;
}
