#ifndef GRAPH_H
#define GRAPH_H

// Include library string untuk menyimpan nama hub/kota
#include <string>
// Include library vector untuk menyimpan list edge dan hasil traversal
#include <vector>
// Include library queue untuk std::queue (digunakan dalam BFS)
#include <queue>   // untuk BFS (std::queue)
// Include library stack untuk std::stack (digunakan dalam DFS)
#include <stack>   // untuk DFS (std::stack)
// Include library map untuk menyimpan adjacency list dan tracking parent
#include <map>
// Include library set untuk tracking visited vertices dalam BFS/DFS
#include <set>
// Include library climits untuk konstanta INT_MAX
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

// STRUCT: Representasi edge dalam graph dengan destination dan weight
struct RouteEdge {
    // Nama hub tujuan dari edge ini
    std::string to;
    // Bobot edge yaitu estimasi waktu tempuh dalam jam
    int         weight; // estimasi waktu dalam jam
};

// CLASS: Jaringan hub logistik menggunakan adjacency list
class Graph {
private:
    // Adjacency List: map dari nama hub → vector edge keluar
    std::map<std::string, std::vector<RouteEdge>> adjList;

    // Daftar semua vertex (hub) untuk iterasi
    std::vector<std::string> vertices;

    // Cek apakah vertex sudah terdaftar
    bool hasVertex(const std::string& v) const {
        // Return true jika vertex ada di adjList
        return adjList.find(v) != adjList.end();
    }

    // Tambah vertex jika belum ada
    void ensureVertex(const std::string& v) {
        // Jika vertex belum terdaftar
        if (!hasVertex(v)) {
            // Buat entry baru dengan vector edge kosong
            adjList[v] = {};
            // Tambahkan ke list vertices
            vertices.push_back(v);
        }
    }

public:
    // KONSTRUKTOR: Inisialisasi graph kosong
    Graph() {}

    // PUBLIC METHOD: Tambah edge berarah dari hub 'from' ke hub 'to' dengan weight (waktu tempuh)
    void addRoute(const std::string& from, const std::string& to, int weight) {
        // Pastikan vertex 'from' terdaftar dalam graph
        ensureVertex(from);
        // Pastikan vertex 'to' terdaftar dalam graph
        ensureVertex(to);
        // Tambahkan edge ke adjacency list 'from'
        adjList[from].push_back({to, weight});
    }

    // PUBLIC METHOD: Tampilkan seluruh jaringan hub dalam format text yang readable
    std::string getNetworkString() const {
        // Inisialisasi output string dengan header
        std::string out = "=== KONEKSI JARINGAN HUB LOGISTIK (GRAPH) ===\n\n";
        // Iterasi semua vertex (hub) dalam graph
        for (const auto& v : vertices) {
            // Tambahkan nama hub dengan emoji 📍
            out += "📍 " + v + "\n";
            // Iterasi semua edge keluar dari hub tersebut
            for (const auto& e : adjList.at(v)) {
                // Format: "➔ hub_tujuan (Estimasi: X Jam)"
                out += "    ➔ " + e.to +
                       "  (Estimasi: " + std::to_string(e.weight) + " Jam)\n";
            }
        }
        // Kembalikan string representasi jaringan
        return out;
    }

    // BFS — Breadth-First Search untuk menemukan rute dengan hop minimum
    // Parameter: start = hub awal, end = hub tujuan
    // Return: String berisi jalur, total waktu, dan jumlah hop
    std::string runBFS(const std::string& start, const std::string& end) const {
        // Validasi: cek apakah start vertex ada dalam graph
        if (!hasVertex(start)) return "❌ Vertex awal tidak ditemukan: " + start;
        // Validasi: cek apakah end vertex ada dalam graph
        if (!hasVertex(end))   return "❌ Vertex tujuan tidak ditemukan: " + end;
        // Jika start == end, tidak perlu traverse
        if (start == end)      return "✅ Sudah di lokasi tujuan: " + start;

        // BFS menggunakan std::queue dengan FIFO (First In First Out)
        std::queue<std::string>              bfsQueue;  // Queue untuk BFS traversal
        std::map<std::string, std::string>   parent;    // Tracking parent node untuk rekonstruksi jalur
        std::map<std::string, int>           cost;      // Tracking total cost/waktu dari start
        std::set<std::string>                visited;   // Tracking vertex yang sudah dikunjungi

        // Inisialisasi: masukkan start vertex ke queue
        bfsQueue.push(start);
        // Mark start sebagai visited
        visited.insert(start);
        // Start vertex tidak punya parent
        parent[start] = "";
        // Cost dari start ke start = 0
        cost[start]   = 0;

        // Flag untuk tracking apakah end vertex ditemukan
        bool found = false;
        // Loop selama queue tidak kosong
        while (!bfsQueue.empty()) {
            // Ambil vertex di depan queue (FIFO)
            std::string curr = bfsQueue.front();
            bfsQueue.pop();

            // Jika vertex saat ini adalah end vertex
            if (curr == end) { found = true; break; }

            // Iterasi semua edge (tetangga) dari vertex saat ini
            for (const auto& edge : adjList.at(curr)) {
                // Jika tetangga belum dikunjungi
                if (visited.find(edge.to) == visited.end()) {
                    // Mark tetangga sebagai visited
                    visited.insert(edge.to);
                    // Set parent tetangga = vertex saat ini
                    parent[edge.to] = curr;
                    // Hitung cost ke tetangga = cost dari start ke curr + weight dari curr ke tetangga
                    cost[edge.to]   = cost[curr] + edge.weight;
                    // Masukkan tetangga ke queue untuk process selanjutnya
                    bfsQueue.push(edge.to);
                }
            }
        }

        // Jika end vertex tidak ditemukan
        if (!found) return "❌ Tidak ada rute dari " + start + " ke " + end;

        // Rekonstruksi jalur dari end ke start menggunakan parent tracking
        std::vector<std::string> path;
        // Mulai dari end vertex
        std::string curr = end;
        // Trace kembali ke start menggunakan parent map
        while (!curr.empty()) {
            // Tambahkan vertex saat ini ke path
            path.push_back(curr);
            // Move ke parent vertex
            curr = parent[curr];
        }
        // Reverse path agar dari start ke end (bukan sebaliknya)
        std::reverse(path.begin(), path.end());

        // Format output hasil BFS
        std::string result = "=== HASIL BFS (Rute Terdekat - Minimum Hop) ===\n";
        result += "Dari    : " + start + "\n";
        result += "Tujuan  : " + end   + "\n";
        result += "Jalur   : ";
        // Print jalur dengan arrow separator
        for (int i = 0; i < (int)path.size(); ++i) {
            result += path[i];
            // Tambahkan arrow kecuali di vertex terakhir
            if (i + 1 < (int)path.size()) result += " ➔ ";
        }
        // Tambahkan total waktu
        result += "\nTotal Waktu : " + std::to_string(cost[end]) + " Jam";
        // Tambahkan jumlah hop (= path.size() - 1 karena start tidak dihitung)
        result += "\nJumlah Hop  : " + std::to_string((int)path.size() - 1) + " titik transit";
        // Kembalikan hasil BFS
        return result;
    }

    // DFS — Depth-First Search untuk audit konektivitas hub dari start vertex
    // Menggunakan explicit stack (bukan rekursi)
    // Parameter: start = hub awal untuk audit
    // Return: String berisi urutan kunjungan dan daftar hub yang tidak terjangkau
    std::string runDFS(const std::string& start) const {
        // Validasi: cek apakah start vertex ada dalam graph
        if (!hasVertex(start)) return "❌ Vertex awal tidak ditemukan: " + start;

        // DFS menggunakan std::stack dengan LIFO (Last In First Out)
        std::stack<std::string>  dfsStack;          // Stack untuk DFS traversal
        std::set<std::string>    visited;           // Tracking vertex yang sudah dikunjungi
        std::vector<std::string> traversalOrder;    // Menyimpan urutan kunjungan vertex

        // Inisialisasi: masukkan start vertex ke stack
        dfsStack.push(start);

        // Loop selama stack tidak kosong
        while (!dfsStack.empty()) {
            // Ambil vertex di top stack (LIFO)
            std::string curr = dfsStack.top();
            dfsStack.pop();

            // Jika vertex sudah dikunjungi, skip
            if (visited.find(curr) != visited.end()) continue;
            // Mark vertex sebagai visited
            visited.insert(curr);
            // Tambahkan ke urutan kunjungan
            traversalOrder.push_back(curr);

            // Ambil daftar tetangga (neighbor) dari vertex saat ini
            const auto& neighbors = adjList.at(curr);
            // Iterasi tetangga dalam urutan terbalik agar urutan process sesuai
            for (int i = (int)neighbors.size() - 1; i >= 0; --i) {
                // Jika tetangga belum dikunjungi
                if (visited.find(neighbors[i].to) == visited.end()) {
                    // Masukkan tetangga ke stack untuk process selanjutnya
                    dfsStack.push(neighbors[i].to);
                }
            }
        }

        // Format output hasil DFS
        std::string result = "=== HASIL DFS (Audit Konektivitas dari " + start + ") ===\n";
        result += "Urutan Kunjungan:\n";
        // Print urutan kunjungan vertex dengan nomor
        for (int i = 0; i < (int)traversalOrder.size(); ++i) {
            result += "  " + std::to_string(i + 1) + ". " + traversalOrder[i] + "\n";
        }
        // Print total hub yang terjangkau
        result += "\nTotal Hub Terjangkau : " + std::to_string(traversalOrder.size());

        // Cari hub yang TIDAK terjangkau dari start
        std::vector<std::string> unreachable;
        // Iterasi semua vertex dalam graph
        for (const auto& v : vertices) {
            // Jika vertex tidak ada dalam visited set
            if (visited.find(v) == visited.end()) 
                // Tambahkan ke list unreachable
                unreachable.push_back(v);
        }
        // Jika tidak ada hub yang unreachable
        if (unreachable.empty()) {
            // Tampilkan pesan sukses
            result += "\n✅ Semua hub dalam jaringan dapat dijangkau dari " + start;
        } 
        // Jika ada hub yang unreachable
        else {
            // Tampilkan pesan warning dan daftar hub unreachable
            result += "\n⚠️ Hub tidak terjangkau: ";
            for (auto& u : unreachable) result += u + " ";
        }
        // Kembalikan hasil DFS
        return result;
    }

    // PUBLIC METHOD: Getter untuk jumlah vertex dalam graph
    int vertexCount() const { return (int)vertices.size(); }

    // PUBLIC METHOD: Getter untuk jumlah edge dalam graph
    int edgeCount() const {
        // Inisialisasi counter edge
        int count = 0;
        // Iterasi semua vertex
        for (auto& v : vertices) 
            // Tambahkan jumlah edge yang keluar dari vertex
            count += (int)adjList.at(v).size();
        // Kembalikan total jumlah edge
        return count;
    }
};

// Akhir guard preprocessor
#endif // GRAPH_H