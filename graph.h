#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <queue>   // untuk BFS (std::queue)
#include <stack>   // untuk DFS (std::stack)
#include <map>
#include <set>
#include <climits>

// =========================================================
// GRAPH + BFS + DFS — Simulasi Rute Pengiriman Logistik
//
// Representasi: Adjacency List (map nama → list tetangga)
// Vertex = kota/hub logistik
// Edge   = jalur pengiriman dengan bobot (estimasi jam)
//
// BFS (Queue-based): cari rute dengan HOP paling sedikit
//   → cocok untuk estimasi rute terdekat ke tujuan
//
// DFS (Stack-based): audit konektivitas semua kota
//   → cocok untuk memeriksa apakah semua hub terjangkau
// =========================================================

struct RouteEdge {
    std::string to;
    int         weight; // estimasi waktu dalam jam
};

class Graph {
private:
    // Adjacency List: nama hub → daftar edge keluar
    std::map<std::string, std::vector<RouteEdge>> adjList;

    // Daftar semua vertex (untuk iterasi)
    std::vector<std::string> vertices;

    // Cek apakah vertex sudah terdaftar
    bool hasVertex(const std::string& v) const {
        return adjList.find(v) != adjList.end();
    }

    // Tambah vertex jika belum ada
    void ensureVertex(const std::string& v) {
        if (!hasVertex(v)) {
            adjList[v] = {};
            vertices.push_back(v);
        }
    }

public:
    Graph() {}

    // Tambah rute berarah (directed edge) antara dua hub
    void addRoute(const std::string& from, const std::string& to, int weight) {
        ensureVertex(from);
        ensureVertex(to);
        adjList[from].push_back({to, weight});
    }

    // Tampilkan seluruh jaringan hub (adjacency list)
    std::string getNetworkString() const {
        std::string out = "=== KONEKSI JARINGAN HUB LOGISTIK (GRAPH) ===\n\n";
        for (const auto& v : vertices) {
            out += "📍 " + v + "\n";
            for (const auto& e : adjList.at(v)) {
                out += "    ➔ " + e.to +
                       "  (Estimasi: " + std::to_string(e.weight) + " Jam)\n";
            }
        }
        return out;
    }

    // -------------------------------------------------------
    // BFS — Breadth-First Search (Queue-based)
    // Menemukan rute dengan jumlah hop paling sedikit
    // dari 'start' ke 'end', lalu tampilkan jalur + total waktu
    // -------------------------------------------------------
    std::string runBFS(const std::string& start, const std::string& end) const {
        if (!hasVertex(start)) return "❌ Vertex awal tidak ditemukan: " + start;
        if (!hasVertex(end))   return "❌ Vertex tujuan tidak ditemukan: " + end;
        if (start == end)      return "✅ Sudah di lokasi tujuan: " + start;

        // BFS menggunakan std::queue (FIFO)
        std::queue<std::string>              bfsQueue;
        std::map<std::string, std::string>   parent;   // untuk rekonstruksi jalur
        std::map<std::string, int>           cost;     // total biaya/waktu
        std::set<std::string>                visited;

        bfsQueue.push(start);
        visited.insert(start);
        parent[start] = "";
        cost[start]   = 0;

        bool found = false;
        while (!bfsQueue.empty()) {
            std::string curr = bfsQueue.front();
            bfsQueue.pop();

            if (curr == end) { found = true; break; }

            for (const auto& edge : adjList.at(curr)) {
                if (visited.find(edge.to) == visited.end()) {
                    visited.insert(edge.to);
                    parent[edge.to] = curr;
                    cost[edge.to]   = cost[curr] + edge.weight;
                    bfsQueue.push(edge.to);
                }
            }
        }

        if (!found) return "❌ Tidak ada rute dari " + start + " ke " + end;

        // Rekonstruksi jalur dari end ke start (lalu balik)
        std::vector<std::string> path;
        std::string curr = end;
        while (!curr.empty()) {
            path.push_back(curr);
            curr = parent[curr];
        }
        std::reverse(path.begin(), path.end());

        std::string result = "=== HASIL BFS (Rute Terdekat - Minimum Hop) ===\n";
        result += "Dari    : " + start + "\n";
        result += "Tujuan  : " + end   + "\n";
        result += "Jalur   : ";
        for (int i = 0; i < (int)path.size(); ++i) {
            result += path[i];
            if (i + 1 < (int)path.size()) result += " ➔ ";
        }
        result += "\nTotal Waktu : " + std::to_string(cost[end]) + " Jam";
        result += "\nJumlah Hop  : " + std::to_string((int)path.size() - 1) + " titik transit";
        return result;
    }

    // -------------------------------------------------------
    // DFS — Depth-First Search (Stack-based)
    // Audit konektivitas: jelajahi semua hub yang bisa dicapai
    // dari 'start', gunakan Stack eksplisit (bukan rekursi)
    // -------------------------------------------------------
    std::string runDFS(const std::string& start) const {
        if (!hasVertex(start)) return "❌ Vertex awal tidak ditemukan: " + start;

        // DFS menggunakan std::stack (LIFO)
        std::stack<std::string>  dfsStack;
        std::set<std::string>    visited;
        std::vector<std::string> traversalOrder;

        dfsStack.push(start);

        while (!dfsStack.empty()) {
            std::string curr = dfsStack.top();
            dfsStack.pop();

            if (visited.find(curr) != visited.end()) continue;
            visited.insert(curr);
            traversalOrder.push_back(curr);

            // Push tetangga ke stack (urutan terbalik agar kiri diproses dulu)
            const auto& neighbors = adjList.at(curr);
            for (int i = (int)neighbors.size() - 1; i >= 0; --i) {
                if (visited.find(neighbors[i].to) == visited.end()) {
                    dfsStack.push(neighbors[i].to);
                }
            }
        }

        std::string result = "=== HASIL DFS (Audit Konektivitas dari " + start + ") ===\n";
        result += "Urutan Kunjungan:\n";
        for (int i = 0; i < (int)traversalOrder.size(); ++i) {
            result += "  " + std::to_string(i + 1) + ". " + traversalOrder[i] + "\n";
        }
        result += "\nTotal Hub Terjangkau : " + std::to_string(traversalOrder.size());

        // Periksa hub yang TIDAK terjangkau
        std::vector<std::string> unreachable;
        for (const auto& v : vertices) {
            if (visited.find(v) == visited.end()) unreachable.push_back(v);
        }
        if (unreachable.empty()) {
            result += "\n✅ Semua hub dalam jaringan dapat dijangkau dari " + start;
        } else {
            result += "\n⚠️ Hub tidak terjangkau: ";
            for (auto& u : unreachable) result += u + " ";
        }
        return result;
    }

    // Getter jumlah vertex dan edge
    int vertexCount() const { return (int)vertices.size(); }

    int edgeCount() const {
        int count = 0;
        for (auto& v : vertices) count += (int)adjList.at(v).size();
        return count;
    }
};

#endif // GRAPH_H