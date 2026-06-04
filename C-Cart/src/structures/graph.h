#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
#include <list>

/**
 * @brief Graph data structure for delivery route calculations.
 * Manual implementation using adjacency list.
 */
class Graph {
private:
    std::map<std::string, std::list<std::string>> adjList;

public:
    Graph();

    /**
     * @brief Add a city to the graph
     */
    void addCity(std::string city);

    /**
     * @brief Add a bidirectional edge between two cities
     */
    void addEdge(std::string from, std::string to);

    /**
     * @brief Find the shortest route between two cities using BFS
     * @return Vector of city names in the shortest path
     */
    std::vector<std::string> bfsShortestRoute(std::string start, std::string destination);

    /**
     * @brief Traverse the graph using DFS
     * @return Vector of city names in traversal order
     */
    std::vector<std::string> dfsTraversal(std::string start);

    /**
     * @brief Check if all cities are connected (using BFS from Jakarta)
     */
    bool isConnected();

    /**
     * @brief Display the graph's adjacency list
     */
    void displayGraph() const;
};

#endif // GRAPH_H
