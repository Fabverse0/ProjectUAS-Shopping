#include "graph.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <set>

using namespace std;

Graph::Graph() {}

void Graph::addCity(string city) {
    if (adjList.find(city) == adjList.end()) {
        adjList[city] = list<string>();
    }
}

void Graph::addEdge(string from, string to) {
    addCity(from);
    addCity(to);
    adjList[from].push_back(to);
    adjList[to].push_back(from);
}

vector<string> Graph::bfsShortestRoute(string start, string destination) {
    if (adjList.find(start) == adjList.end() || adjList.find(destination) == adjList.end()) {
        return {};
    }

    queue<string> q;
    map<string, string> parent;
    set<string> visited;

    q.push(start);
    visited.insert(start);
    parent[start] = "";

    bool found = false;
    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == destination) {
            found = true;
            break;
        }

        for (const string& neighbor : adjList[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    if (!found) return {};

    vector<string> path;
    string current = destination;
    while (current != "") {
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<string> Graph::dfsTraversal(string start) {
    if (adjList.find(start) == adjList.end()) return {};

    vector<string> result;
    stack<string> s;
    set<string> visited;

    s.push(start);

    while (!s.empty()) {
        string current = s.top();
        s.pop();

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            result.push_back(current);

            for (auto it = adjList[current].rbegin(); it != adjList[current].rend(); ++it) {
                if (visited.find(*it) == visited.end()) {
                    s.push(*it);
                }
            }
        }
    }
    return result;
}

bool Graph::isConnected() {
    if (adjList.empty()) return true;
    
    string startCity = adjList.begin()->first;
    vector<string> traversal = dfsTraversal(startCity);
    
    return traversal.size() == adjList.size();
}

void Graph::display() const {
    cout << "--- Peta Kota (Graph) ---" << endl;
    for (auto const& [city, neighbors] : adjList) {
        cout << city << " -> ";
        bool first = true;
        for (const string& neighbor : neighbors) {
            if (!first) cout << ", ";
            cout << neighbor;
            first = false;
        }
        cout << endl;
    }
}
