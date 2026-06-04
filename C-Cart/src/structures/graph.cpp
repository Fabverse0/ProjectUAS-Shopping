#include "graph.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <set>

Graph::Graph() {}

void Graph::addCity(std::string city) {
    if (adjList.find(city) == adjList.end()) {
        adjList[city] = std::list<std::string>();
    }
}

void Graph::addEdge(std::string from, std::string to) {
    addCity(from);
    addCity(to);
    adjList[from].push_back(to);
    adjList[to].push_back(from);
}

std::vector<std::string> Graph::bfsShortestRoute(std::string start, std::string destination) {
    if (adjList.find(start) == adjList.end() || adjList.find(destination) == adjList.end()) {
        return {};
    }

    std::queue<std::string> q;
    std::map<std::string, std::string> parent;
    std::set<std::string> visited;

    q.push(start);
    visited.insert(start);
    parent[start] = "";

    bool found = false;
    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        if (current == destination) {
            found = true;
            break;
        }

        for (const std::string& neighbor : adjList[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    if (!found) return {};

    std::vector<std::string> path;
    std::string current = destination;
    while (current != "") {
        path.push_back(current);
        current = parent[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<std::string> Graph::dfsTraversal(std::string start) {
    if (adjList.find(start) == adjList.end()) return {};

    std::vector<std::string> result;
    std::stack<std::string> s;
    std::set<std::string> visited;

    s.push(start);

    while (!s.empty()) {
        std::string current = s.top();
        s.pop();

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            result.push_back(current);

            // Using list's reverse iterator to match typical DFS behavior
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
    
    std::string startCity = adjList.begin()->first;
    std::vector<std::string> traversal = dfsTraversal(startCity);
    
    return traversal.size() == adjList.size();
}

void Graph::displayGraph() const {
    std::cout << "--- Peta Kota (Graph) ---" << std::endl;
    for (auto const& [city, neighbors] : adjList) {
        std::cout << city << " -> ";
        bool first = true;
        for (const std::string& neighbor : neighbors) {
            if (!first) std::cout << ", ";
            std::cout << neighbor;
            first = false;
        }
        std::cout << std::endl;
    }
}
