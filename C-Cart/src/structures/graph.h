#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
#include <list>

// Struktur data Graph untuk perhitungan rute pengiriman kota
class Graph {
private:
    std::map<std::string, std::list<std::string>> adjList;

public:
    Graph();

    // Menambah kota ke dalam graph
    void addCity(std::string city);

    // Menambah jalur dua arah antar kota
    void addEdge(std::string from, std::string to);

    // Mencari rute terpendek dengan BFS
    std::vector<std::string> bfsShortestRoute(std::string start, std::string destination);

    // Penelusuran semua kota dengan DFS
    std::vector<std::string> dfsTraversal(std::string start);

    // Mengecek apakah semua kota terhubung
    bool isConnected();

    // Menampilkan adjacency list
    void display() const;
};

#endif // GRAPH_H
