// =========================================================
// FILE: mainwindow.h
// KETERANGAN: Header file untuk jendela utama aplikasi shopping
//             Mendefinisikan struktur data dan class utama MainWindow
// =========================================================

#ifndef MAINWINDOW_H  // Guard predprosesor untuk mencegah include ganda
#define MAINWINDOW_H  // Definisi guard header

// =========================================================
// INCLUDE LIBRARY Qt — UI COMPONENTS
// =========================================================
#include <QMainWindow>    // Class dasar untuk jendela aplikasi utama
#include <QHBoxLayout>    // Layout horizontal untuk mengatur widget horizontal
#include <QVBoxLayout>    // Layout vertikal untuk mengatur widget vertikal
#include <QFormLayout>    // Layout form untuk label + input fields
#include <QFrame>         // Class untuk membuat frame/panel visual
#include <QLabel>         // Widget untuk menampilkan text/gambar statis
#include <QPushButton>    // Widget tombol yang dapat diklik
#include <QStackedWidget> // Widget container untuk multiple pages (tab-like behavior)
#include <QGroupBox>      // Widget untuk mengelompokkan controls dengan border
#include <QListWidget>    // Widget list untuk menampilkan daftar item
#include <QTextEdit>      // Widget text editor multi-line untuk menampilkan/edit text
#include <QLineEdit>      // Widget single-line input untuk user input text
#include <QComboBox>      // Widget dropdown/combo box untuk pilihan
#include <QDateTime>      // Class untuk handling date & time
#include <QTimer>         // Class timer untuk menjalankan fungsi secara periodik
#include <QTableWidget>   // Widget table untuk menampilkan data dalam format grid
#include <QHeaderView>    // Class untuk mengatur header (baris/kolom) di table
#include <QSpinBox>       // Widget spinner untuk input angka

// =========================================================
// INCLUDE LIBRARY C++ STANDARD — DATA STRUCTURES & UTILITIES
// =========================================================
#include <vector>         // Dynamic array container (std::vector)
#include <string>         // String handling (std::string)
#include <map>            // Key-value pair container (std::map)
#include <algorithm>      // Fungsi algoritma (sort, find, reverse, dll)
#include <cctype>         // Character type functions (tolower, toupper, dll)
#include <queue>          // Queue container untuk BFS (FIFO — First In First Out)
#include <stack>          // Stack container untuk DFS (LIFO — Last In First Out)
#include <set>            // Set container untuk data unik tanpa duplikat


// =========================================================
// INCLUDE STRUKTUR DATA TAMBAHAN
// =========================================================
#include "nav_stack.h"      // No.4: Stack — Riwayat Navigasi halaman sebelumnya
#include "binary_search.h"  // No.6: Binary Search — Pencarian produk berdasarkan ID
#include "bst.h"            // No.7: BST — Mengurutkan harga katalog (ascending/descending)
#include "avl_tree.h"       // No.8: AVL Tree — Penyimpanan produk berdasarkan ID dengan self-balancing
#include "hash_table.h"     // No.9: Hash Table — Quick lookup O(1) untuk product/voucher/order

// =========================================================
// STRUKTUR DATA: CIRCULAR LINKED LIST (Banner Promo)
// No. 1: Circular Linked List — Rotasi Banner Promosi
// KETERANGAN: Digunakan untuk menyimpan text promo yang berrotasi
//             Setiap node menunjuk ke node berikutnya, node terakhir menunjuk kembali ke head
// =========================================================
struct PromoNode {
    std::string teks;         // Text konten promo yang akan ditampilkan di banner
    PromoNode* next = nullptr; // Pointer ke node berikutnya (circular — menunjuk kembali ke head pada akhir)
};

class PromoBanner {
private:
    PromoNode* head    = nullptr;  // Pointer ke node pertama/awal circular list
    PromoNode* current = nullptr;  // Pointer ke node yang sedang aktif/ditampilkan
    int total          = 0;        // Counter total promo dalam list

public:
    ~PromoBanner() {  // Destructor: membersihkan semua node saat object dihapus
        if (!head) return;  // Jika list kosong, return langsung
        // Putus siklus dulu sebelum delete agar tidak infinite loop saat deletion
        PromoNode* temp = head;
        do {  // Do-while karena list circular, minimal 1 iterasi
            PromoNode* next = temp->next;  // Simpan pointer ke node berikutnya
            delete temp;                    // Hapus node saat ini
            temp = next;                    // Pindah ke node berikutnya
        } while (temp != head);  // Hentikan ketika kembali ke head
    }

    // Fungsi menambah promo baru ke circular linked list
    void addPromo(const std::string& teks) {
        PromoNode* node = new PromoNode{teks, nullptr};  // Buat node baru dengan text promo
        if (!head) {  // Jika list masih kosong (penambahan pertama)
            head          = node;     // Set node sebagai head
            node->next    = head;     // Node menunjuk ke dirinya sendiri (circular dengan 1 node)
            current       = head;     // Set current ke node yang baru ditambah
        } else {  // Jika list sudah ada node sebelumnya
            // Cari node terakhir (yang next-nya == head, menandakan akhir circular list)
            PromoNode* tail = head;
            while (tail->next != head) tail = tail->next;  // Iterasi sampai menemukan tail
            tail->next = node;      // Sambungkan node baru setelah tail
            node->next = head;      // Node baru menunjuk ke head (pertahankan circular)
        }
        total++;  // Increment counter total promo
    }

    // Fungsi untuk maju ke node berikutnya (untuk rotasi banner)
    // Karena circular, setelah node terakhir otomatis kembali ke head
    void moveNext() {
        if (current) current = current->next;  // Geser current ke node berikutnya
    }

    // Fungsi untuk mendapatkan text promo yang sedang aktif
    std::string getTeks() const {
        return current ? current->teks : "";  // Return text dari node current, atau empty string jika null
    }

    // Fungsi untuk mengecek apakah banner list kosong
    bool isEmpty() const { return head == nullptr; }  // Return true jika head null (list kosong)
};

// Qt macro untuk namespace declaration (internal Qt usage)
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }  // Forward declaration dari UI class yang auto-generated
QT_END_NAMESPACE

// =========================================================
// ENUM: ROLE LOGIN — Untuk membedakan tipe user
// =========================================================
enum class UserRole { NONE, PELANGGAN, ADMIN };  // NONE=belum login, PELANGGAN=customer, ADMIN=administrator


// =========================================================
// STRUKTUR DATA: DOUBLE & SINGLE LINKED LIST
// No. 2: Double Linked List (Katalog) — Navigasi forward/backward
// No. 3: Single Linked List (Keranjang) — List linear sederhana
// =========================================================

// Node untuk Double Linked List (Katalog Produk)
struct ProductNode {
    int id;                  // ID unik produk
    std::string name;        // Nama produk
    std::string category;    // Kategori produk (Elektronik/Pakaian/Perabotan)
    long price;              // Harga produk dalam Rupiah
    std::string origin;      // Asal/Hub produk (Jakarta/Surabaya/Bandung, dll)
    ProductNode* next = nullptr;  // Pointer ke node berikutnya (forward navigation)
    ProductNode* prev = nullptr;  // Pointer ke node sebelumnya (backward navigation) — untuk double linked list
};

// Class Double Linked List untuk menyimpan katalog produk
// Memungkinkan navigasi maju (next) dan mundur (prev)
class CatalogDLL {
private:
    ProductNode* head = nullptr;    // Pointer ke node pertama katalog
    ProductNode* tail = nullptr;    // Pointer ke node terakhir katalog
    ProductNode* current = nullptr; // Pointer ke node yang sedang aktif/ditampilkan user

public:
    CatalogDLL() : head(nullptr), tail(nullptr), current(nullptr) {}  // Constructor inisialisasi semua pointer ke null
    
    ~CatalogDLL() {  // Destructor: membersihkan semua node
        ProductNode* temp = head;
        while (temp != nullptr) {
            ProductNode* nextNode = temp->next;  // Simpan pointer node berikutnya
            delete temp;                         // Hapus node saat ini
            temp = nextNode;                     // Pindah ke node berikutnya
        }
    }

    // Fungsi menambah produk baru ke akhir catalog (insertion di tail)
    void insertProduct(int id, std::string name, std::string cat, long price, std::string orig) {
        ProductNode* newNode = new ProductNode{id, name, cat, price, orig, nullptr, nullptr};
        if (head == nullptr) {  // Jika list kosong (pertama kali insert)
            head = tail = current = newNode;  // Set semua pointer ke node baru
        }
        else {  // Jika list sudah ada node
            tail->next = newNode;      // Link node terakhir (tail) ke node baru
            newNode->prev = tail;      // Link node baru ke node terakhir (untuk double link)
            tail = newNode;            // Update tail ke node baru
        }
    }

    // Fungsi mendapatkan node yang sedang aktif
    ProductNode* getCurrentNode() { return current; }
    
    // Fungsi navigasi maju ke produk berikutnya
    void moveNext() { if (current && current->next) current = current->next; }
    
    // Fungsi navigasi mundur ke produk sebelumnya
    void movePrev() { if (current && current->prev) current = current->prev; }

    // Fungsi mendapatkan semua produk dalam urutan harga (ascending/descending)
    std::vector<ProductNode*> getSortedByPrice(bool asc) {
        std::vector<ProductNode*> list;
        ProductNode* temp = head;
        while (temp != nullptr) {  // Traverse list, copy pointer setiap node ke vector
            list.push_back(temp);
            temp = temp->next;
        }
        // Sort menggunakan lambda comparator (asc = true untuk ascending, false untuk descending)
        std::sort(list.begin(), list.end(), [asc](ProductNode* a, ProductNode* b) {
            return asc ? (a->price < b->price) : (a->price > b->price);
        });
        return list;
    }

    // Fungsi pencarian produk berdasarkan substring (case-insensitive)
    std::vector<ProductNode*> searchProductBySubstring(std::string kw) {
        std::vector<ProductNode*> res;
        // Convert keyword ke lowercase untuk case-insensitive search
        std::string lowerKw = kw;
        std::transform(lowerKw.begin(), lowerKw.end(), lowerKw.begin(), ::tolower);
        
        ProductNode* temp = head;
        while (temp != nullptr) {
            // Convert nama produk ke lowercase
            std::string lowerName = temp->name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            
            // Cek apakah keyword ada dalam nama produk (substring search)
            if (lowerName.find(lowerKw) != std::string::npos) res.push_back(temp);
            temp = temp->next;
        }
        return res;
    }

    // Fungsi hapus semua node dan reset pointer (digunakan saat rebuild katalog)
    void clearCatalog() {
        while (head) {
            ProductNode* tmp = head;
            head = head->next;  // Geser head ke node berikutnya
            delete tmp;         // Hapus node lama
        }
        tail = current = nullptr;  // Reset tail dan current ke null
    }
};

// Class Single Linked List untuk menyimpan item keranjang belanja
// Single linked list lebih sederhana karena hanya bisa navigate forward
class CartSLL {
private:
    ProductNode* head = nullptr;  // Pointer ke node pertama keranjang
    
public:
    CartSLL() : head(nullptr) {}  // Constructor inisialisasi head ke null
    
    ~CartSLL() { clearList(); }  // Destructor panggil clearList() untuk cleanup

    // Fungsi menambah item ke akhir keranjang (insertion di tail)
    void insertItem(int id, std::string name, long price, std::string orig) {
        ProductNode* newNode = new ProductNode{id, name, "", price, orig, nullptr, nullptr};
        if (!head) {  // Jika keranjang kosong
            head = newNode;
        }
        else {  // Jika keranjang sudah ada item
            ProductNode* temp = head;
            while (temp->next) temp = temp->next;  // Traverse ke akhir list (tail)
            temp->next = newNode;  // Link tail ke node baru
        }
    }

    // Fungsi menghapus item pada indeks tertentu
    void deleteNodeAt(int idx) {
        if (!head) return;  // Jika keranjang kosong, return
        
        ProductNode* temp = head;
        if (idx == 0) {  // Jika menghapus node pertama (head)
            head = head->next;
            delete temp;
            return;
        }
        
        // Traverse ke node sebelum target (idx-1)
        for (int i = 0; temp != nullptr && i < idx - 1; i++) temp = temp->next;
        
        if (!temp || !temp->next) return;  // Index out of bounds
        
        ProductNode* next = temp->next->next;  // Simpan pointer ke node setelah target
        delete temp->next;                      // Hapus node target
        temp->next = next;                      // Skip node yang dihapus
    }

    // Fungsi menghitung total harga semua item di keranjang
    long calculateTotal() {
        long total = 0;
        ProductNode* temp = head;
        while (temp) {
            total += temp->price;  // Akumulasi harga setiap item
            temp = temp->next;
        }
        return total;
    }

    // Fungsi mengecek apakah keranjang kosong
    bool isEmpty() { return head == nullptr; }

    // Fungsi menghapus semua item (clear keranjang)
    void clearList() {
        while (head) {
            ProductNode* temp = head;
            head = head->next;  // Geser head
            delete temp;        // Hapus node lama
        }
    }

    // Fungsi mendapatkan semua item sebagai string (format: "Nama (Rp XXXX)")
    std::vector<std::string> getAllItemsString() {
        std::vector<std::string> res;
        ProductNode* temp = head;
        while (temp) {
            res.push_back(temp->name + " (Rp " + std::to_string(temp->price) + ")");
            temp = temp->next;
        }
        return res;
    }

    // Fungsi mendapatkan semua asal (origin/hub) produk dalam keranjang
    std::vector<std::string> getAllOrigins() {
        std::vector<std::string> res;
        ProductNode* temp = head;
        while (temp) {
            res.push_back(temp->origin);
            temp = temp->next;
        }
        return res;
    }

    // Fungsi mendapatkan semua nama produk dalam keranjang
    std::vector<std::string> getAllNames() {
        std::vector<std::string> res;
        ProductNode* temp = head;
        while (temp) {
            res.push_back(temp->name);
            temp = temp->next;
        }
        return res;
    }

    // Fungsi mendapatkan semua harga produk dalam keranjang
    std::vector<long> getAllPrices() {
        std::vector<long> res;
        ProductNode* temp = head;
        while (temp) {
            res.push_back(temp->price);
            temp = temp->next;
        }
        return res;
    }
};

// =========================================================
// STRUKTUR DATA: QUEUE & GRAPH
// No. 5: Queue — Antrean Order (FIFO: First In First Out)
// No. 10: Graph — Peta Logistik Hub (BFS/DFS untuk routing)
// =========================================================
// Node untuk Queue (Antrean Order)
struct QueueNode {
    std::string idPelanggan;      // ID/username pelanggan pemilik order
    long totalBayar;              // Total harga order dalam Rupiah
    std::string detailNota;       // Detail nota/invoice order
    bool isInstant;               // Flag: true = pengiriman instan, false = reguler
    std::string kotaTujuan;       // Kota tujuan pengiriman
    std::string kotaAsal;         // Kota asal/hub pengiriman
    std::string namaPelanggan;    // Nama lengkap pelanggan
    QueueNode* next = nullptr;    // Pointer ke order berikutnya dalam antrean
};

// Class Queue untuk menyimpan antrean order dengan priority (instan > reguler)
// Menggunakan FIFO (First In First Out) dengan modifikasi prioritas
class LogisticQueue {
private:
    QueueNode* front = nullptr;  // Pointer ke order di depan antrean
    QueueNode* rear  = nullptr;  // Pointer ke order di belakang antrean

public:
    LogisticQueue() : front(nullptr), rear(nullptr) {}  // Constructor inisialisasi antrean kosong
    
    ~LogisticQueue() { while (!isEmpty()) dequeue(); }  // Destructor hapus semua order

    // Fungsi enqueue dengan prioritas: order instan dimasukkan di depan order reguler
    void enqueueOrderPriority(std::string id, long total, std::string detail,
                              bool instant, std::string dest, std::string asal, std::string pemilik) {
        QueueNode* newNode = new QueueNode{id, total, detail, instant, dest, asal, pemilik, nullptr};
        
        if (isEmpty()) {  // Jika antrean kosong, order baru jadi satu-satunya
            front = rear = newNode;
            return;
        }
        
        if (instant) {  // Jika order instan, prioritas tinggi (masuk di depan)
            if (!front->isInstant) {  // Jika order di depan adalah reguler
                newNode->next = front;
                front = newNode;  // Order instan jadi urutan pertama
            }
            else {  // Jika order di depan juga instan
                // Cari posisi untuk insert: setelah semua order instan yang sudah ada
                QueueNode* temp = front;
                while (temp->next != nullptr && temp->next->isInstant) temp = temp->next;
                newNode->next = temp->next;
                temp->next = newNode;
                if (newNode->next == nullptr) rear = newNode;  // Update rear jika insert di akhir
            }
        } else {  // Jika order reguler, prioritas rendah (masuk di belakang)
            rear->next = newNode;
            rear = newNode;  // Update rear ke order baru
        }
    }

    // Fungsi dequeue: hapus order pertama dari antrean
    void dequeue() {
        if (isEmpty()) return;  // Jika antrean kosong, return
        QueueNode* temp = front;
        front = front->next;  // Geser front ke order berikutnya
        if (front == nullptr) rear = nullptr;  // Jika antrean jadi kosong, reset rear
        delete temp;  // Hapus order yang dihapus
    }

    // Fungsi cek antrean kosong
    bool isEmpty() { return front == nullptr; }
    
    // Fungsi ambil detail nota dari order di depan antrean
    std::string peekFrontDetail()      { return isEmpty() ? "" : front->detailNota; }
    
    // Fungsi ambil tujuan dari order di depan antrean
    std::string peekFrontDestination() { return isEmpty() ? "" : front->kotaTujuan; }
    
    // Fungsi ambil asal dari order di depan antrean
    std::string peekFrontOrigin()      { return isEmpty() ? "Hub Jakarta" : front->kotaAsal; }
    
    // Fungsi ambil nama pelanggan dari order di depan antrean
    std::string peekFrontOwner()       { return isEmpty() ? "" : front->namaPelanggan; }
    
    // Fungsi ambil ID pelanggan dari order di depan antrean
    std::string peekFrontId()          { return isEmpty() ? "" : front->idPelanggan; }

    // Fungsi mendapatkan ringkasan seluruh antrean (ID + Total + Label instan/reguler)
    std::vector<std::string> getQueueSummary() {
        std::vector<std::string> summary;
        QueueNode* temp = front;
        while (temp != nullptr) {
            std::string label = temp->isInstant ? "[⚡ INSTAN]" : "[📦 REGULER]";
            summary.push_back(temp->idPelanggan + " | Total: Rp " + std::to_string(temp->totalBayar) + " " + label);
            temp = temp->next;
        }
        return summary;
    }
};

// Class Graph untuk menyimpan peta logistik hub dengan adjacency list
// Digunakan untuk BFS (mencari rute terdekat) dan DFS (audit konektivitas)
class LogisticGraph {
private:
    std::vector<std::string> vertices;                   // Daftar nama kota/hub
    std::vector<std::vector<std::pair<int,int>>> adjList; // Adjacency list: edge = (vertex_index, waktu)

    // Fungsi private: dapatkan index vertex berdasarkan nama kota
    // Jika belum ada, tambahkan vertex baru
    int getVertexIndex(std::string name) {
        for (size_t i = 0; i < vertices.size(); ++i)
            if (vertices[i] == name) return static_cast<int>(i);  // Vertex sudah ada, return index
        
        // Vertex belum ada, tambahkan vertex baru
        vertices.push_back(name);
        adjList.push_back(std::vector<std::pair<int,int>>());
        return static_cast<int>(vertices.size() - 1);  // Return index vertex baru
    }

public:
    // Fungsi menambah edge (rute) antara dua kota dengan waktu tempuh
    void addRoute(std::string u, std::string v, int w) {
        int uIdx = getVertexIndex(u);  // Dapatkan index kota asal
        int vIdx = getVertexIndex(v);  // Dapatkan index kota tujuan
        adjList[uIdx].push_back({vIdx, w});  // Tambah edge ke adjacency list
    }

    // Fungsi mendapatkan string representasi seluruh graph (semua edges)
    std::string getNetworkString() {
        std::string out = "=== JALUR HUB LOGISTIK AKTIF (GRAPH) ===\n";
        for (size_t i = 0; i < vertices.size(); ++i) {
            out += "📍 " + vertices[i] + " terhubung ke:\n";
            for (auto const& edge : adjList[i])
                out += "    ➔ " + vertices[edge.first] + " (Estimasi Waktu: " + std::to_string(edge.second) + " Jam)\n";
        }
        return out;
    }

    // -------------------------------------------------------
    // BFS — Breadth-First Search (Queue-based)
    // FUNGSI: Menemukan rute dengan jumlah hop paling sedikit (shortest path by hops)
    // INPUT: start = kota asal, end = kota tujuan
    // OUTPUT: String berisi jalur, total waktu, dan jumlah hop
    // ALGORITMA: Gunakan queue (FIFO) untuk explore neighbors level per level
    // -------------------------------------------------------
    std::string runBFS(const std::string& start, const std::string& end) {
        if (vertices.empty()) return "❌ Graf kosong.";  // Cek graph tidak kosong

        // Cari index untuk vertex start dan end
        int startIdx = -1, endIdx = -1;
        for (int i = 0; i < (int)vertices.size(); ++i) {
            if (vertices[i] == start) startIdx = i;
            if (vertices[i] == end)   endIdx   = i;
        }
        
        // Validasi vertex ada dalam graph
        if (startIdx < 0) return "❌ Vertex awal tidak ditemukan: " + start;
        if (endIdx   < 0) return "❌ Vertex tujuan tidak ditemukan: " + end;
        if (startIdx == endIdx) return "✅ Sudah di lokasi tujuan: " + start;

        // Inisialisasi array untuk tracking parent, cost, dan visited status
        std::vector<int>  parent(vertices.size(), -1);   // Untuk rekonstruksi jalur
        std::vector<int>  cost(vertices.size(), 0);      // Total waktu dari start ke setiap vertex
        std::vector<bool> visited(vertices.size(), false); // Flag vertex sudah dikunjungi
        std::queue<int>   q;                              // Queue untuk BFS (FIFO)

        // Mulai BFS dari vertex start
        q.push(startIdx);
        visited[startIdx] = true;
        bool found = false;

        while (!q.empty()) {
            int curr = q.front(); q.pop();  // Dequeue vertex dari front queue
            
            if (curr == endIdx) { found = true; break; }  // Target ditemukan, stop BFS
            
            // Explore semua neighbor dari curr
            for (auto& edge : adjList[curr]) {
                if (!visited[edge.first]) {  // Jika neighbor belum dikunjungi
                    visited[edge.first] = true;
                    parent[edge.first]  = curr;              // Track parent untuk jalur
                    cost[edge.first]    = cost[curr] + edge.second;  // Akumulasi cost (waktu)
                    q.push(edge.first);  // Enqueue neighbor
                }
            }
        }

        if (!found) return "❌ Tidak ada rute dari " + start + " ke " + end;

        // Rekonstruksi jalur dari end ke start (lalu balik urutannya)
        std::vector<std::string> path;
        for (int v = endIdx; v != -1; v = parent[v])
            path.push_back(vertices[v]);  // Build path backward
        std::reverse(path.begin(), path.end());  // Balik urutannya (start -> end)

        // Format hasil BFS
        std::string result = "=== HASIL BFS (Rute Terdekat - Minimum Hop) ===\n";
        result += "Dari    : " + start + "\n";
        result += "Tujuan  : " + end   + "\n";
        result += "Jalur   : ";
        for (int i = 0; i < (int)path.size(); ++i) {
            result += path[i];
            if (i + 1 < (int)path.size()) result += " ➔ ";
        }
        result += "\nTotal Waktu : " + std::to_string(cost[endIdx]) + " Jam";
        result += "\nJumlah Hop  : " + std::to_string((int)path.size() - 1) + " titik transit";
        return result;
    }

    // -------------------------------------------------------
    // DFS — Depth-First Search (Stack-based)
    // FUNGSI: Audit konektivitas — jelajahi semua hub yang bisa dicapai dari start
    // INPUT: start = kota asal
    // OUTPUT: String berisi urutan kunjungan dan hub yang tidak terjangkau
    // ALGORITMA: Gunakan stack (LIFO) untuk explore depth-first
    // -------------------------------------------------------
    std::string runDFS(const std::string& start) {
        if (vertices.empty()) return "❌ Graf kosong.";  // Cek graph tidak kosong

        // Cari index untuk vertex start
        int startIdx = -1;
        for (int i = 0; i < (int)vertices.size(); ++i)
            if (vertices[i] == start) { startIdx = i; break; }
        
        if (startIdx < 0) return "❌ Vertex awal tidak ditemukan: " + start;

        // Inisialisasi array untuk tracking visited dan order kunjungan
        std::vector<bool>        visited(vertices.size(), false);  // Flag vertex sudah dikunjungi
        std::vector<std::string> order;                             // Urutan kunjungan vertex
        std::stack<int>          s;                                 // Stack untuk DFS (LIFO)

        s.push(startIdx);  // Push vertex start ke stack
        while (!s.empty()) {
            int curr = s.top(); s.pop();  // Pop vertex dari top stack
            
            if (visited[curr]) continue;  // Skip jika sudah dikunjungi
            
            visited[curr] = true;
            order.push_back(vertices[curr]);  // Record vertex dalam order kunjungan
            
            // Push tetangga ke stack (urutan terbalik agar left-to-right processing)
            for (int i = (int)adjList[curr].size() - 1; i >= 0; --i)
                if (!visited[adjList[curr][i].first])
                    s.push(adjList[curr][i].first);
        }

        // Format hasil DFS
        std::string result = "=== HASIL DFS (Audit Konektivitas dari " + start + ") ===\n";
        result += "Urutan Kunjungan:\n";
        for (int i = 0; i < (int)order.size(); ++i)
            result += "  " + std::to_string(i + 1) + ". " + order[i] + "\n";
        result += "\nTotal Hub Terjangkau : " + std::to_string(order.size());

        // Periksa hub yang TIDAK terjangkau (unreachable)
        std::string unreachable;
        for (int i = 0; i < (int)vertices.size(); ++i)
            if (!visited[i]) unreachable += vertices[i] + " ";
        
        result += unreachable.empty()
                      ? "\n✅ Semua hub dalam jaringan dapat dijangkau dari " + start
                      : "\n⚠️ Hub tidak terjangkau: " + unreachable;
        return result;
    }
};

// =========================================================
// STRUKTUR: ACTIVE ORDER — Tracking order dengan update admin
// KETERANGAN: Menyimpan detail order dan status logistik secara real-time
// =========================================================
struct ActiveOrder {
    std::string idPelanggan;         // ID/username pelanggan pemilik order
    std::string namaPelanggan;       // Nama lengkap pelanggan pemilik order
    std::string detailNota;          // Detail nota/invoice order
    std::string kotaAsal;            // Kota hub asal/gudang produk
    std::string kotaTujuan;          // Kota tujuan pengiriman
    bool isInstant;                  // Flag: true = instan, false = reguler
    std::string statusLogistik;      // Status current: "Dalam Proses", "Sedang Dikirim", "Delivered", dll
    int sisaWaktuJam;                // Estimasi sisa waktu dalam jam
    std::string lokasiSekarang;      // Lokasi terakhir paket (hub mana yang sudah dilewati)
    std::string lastAdminUpdate;     // Timestamp terakhir admin melakukan update
    bool sudahDiUpdateAdmin;         // Flag: hanya tampil ke pelanggan setelah admin melakukan update
};

// =========================================================
// KELAS UTAMA: MAINWINDOW — Jendela Aplikasi Utama
// INHERITANCE: Dari QMainWindow (Qt class untuk top-level window)
// Q_OBJECT: Macro untuk enable Qt Signal/Slot mechanism
// =========================================================
class MainWindow : public QMainWindow  // MainWindow inherit dari QMainWindow (top-level window)
{
    Q_OBJECT  // Macro Qt untuk enable Signal/Slot mechanism

public:
    MainWindow(QWidget *parent = nullptr);  // Constructor: parent widget (default: nullptr)
    ~MainWindow();  // Destructor: cleanup saat window ditutup

private slots:  // Private slot functions — hanya bisa dipanggil dari signals internal
    // ===== SECTION: LOGIN =====
    void klikLogin();        // Slot: handle login button click
    void klikLogout();       // Slot: handle logout button click

    // ===== SECTION: CUSTOMER NAVIGATION =====
    void navigasiHalaman();           // Slot: navigasi antar halaman/pages
    void gantiKategoriKatalog();      // Slot: switch kategori produk (Elektronik/Pakaian/Perabotan)
    void klikNextProduk();            // Slot: navigasi ke produk berikutnya (double linked list)
    void klikPrevProduk();            // Slot: navigasi ke produk sebelumnya (double linked list)
    void klikTambahKeranjang();       // Slot: tambah produk current ke keranjang (cart)
    void klikHapusCart();             // Slot: hapus item dari keranjang
    void klikCheckout();              // Slot: proses checkout order
    void klikAturLayananPerPaket();   // Slot: dialog untuk pilih layanan (instan/reguler) per sub-paket
    void klikUrutHarga();             // Slot: sort katalog berdasarkan harga (BST)
    void klikCariId();                // Slot: cari produk berdasarkan ID (Binary Search)
    void klikBack();                  // Slot: navigasi kembali halaman sebelumnya (Stack)

    // ===== SECTION: CUSTOMER TRACKING =====
    void klikRefreshTrackingPelanggan();  // Slot: refresh tracking status order pelanggan

    // ===== SECTION: PROMO BANNER =====
    void rotasiPromoBanner();  // Slot: rotate banner promo (Circular Linked List)

    // ===== SECTION: ADMIN NAVIGATION =====
    void navigasiAdmin();              // Slot: navigasi ke halaman admin
    void klikProsesOrderAdmin();        // Slot: process order dari queue (dequeue)
    void klikUpdateStatusAdmin();       // Slot: update status order di dashboard admin

    // ===== SECTION: ADMIN PRODUCT MANAGEMENT =====
    void klikTambahProdukAdmin();       // Slot: tambah produk baru (INSERT)
    void klikUpdateProdukAdmin();       // Slot: update data produk yang ada (UPDATE)
    void klikHapusProdukAdmin();        // Slot: hapus produk (DELETE)
    void refreshTabelProdukAdmin();     // Slot: refresh tampilan tabel produk

    // ===== SECTION: ADMIN HASH TABLE LOOKUP =====
    void klikQuickLookup();  // Slot: buka halaman quick lookup hash table

private:
    Ui::MainWindow *ui;  // Pointer ke UI object (auto-generated dari mainwindow.ui)

    // ============================================================
    // SECTION: LOGIN STATE — Menyimpan informasi user yang sedang login
    // ============================================================
    UserRole roleAktif = UserRole::NONE;      // Role user saat ini: NONE/PELANGGAN/ADMIN
    std::string usernameAktif = "";           // Username user yang sedang login

    // ============================================================
    // SECTION: SHARED DATA — Database dan struktur data bersama
    // ============================================================
    std::vector<ActiveOrder> databasePelacakan;  // Database tracking semua order yang aktif

    // ============================================================
    // SECTION: No. 1 — CIRCULAR LINKED LIST (Banner Promo)
    // ============================================================
    PromoBanner promoBanner;     // Instance circular linked list untuk banner promo
    QTimer     *promoTimer = nullptr;  // Timer untuk rotasi banner secara periodik

    // ============================================================
    // SECTION: DOUBLE/SINGLE LINKED LIST (Katalog & Keranjang)
    // ============================================================
    std::string kategoriAktif;   // Kategori produk yang sedang ditampilkan user
    CatalogDLL katalogElektronikDLL;   // Katalog elektronik (double linked list)
    CatalogDLL katalogPakaianDLL;      // Katalog pakaian (double linked list)
    CatalogDLL katalogPerabotanDLL;    // Katalog perabotan (double linked list)
    CartSLL keranjangSLL;              // Keranjang belanja (single linked list)
    LogisticQueue antreanQueue;        // Queue antrean order dengan priority
    LogisticGraph petaLogistik;        // Graph peta logistik hub untuk BFS/DFS

    // ============================================================
    // SECTION: No. 4 — STACK (Riwayat Navigasi Halaman)
    // ============================================================
    NavStack navStack;  // Stack untuk menyimpan riwayat halaman sebelumnya (untuk tombol Back)

    // ============================================================
    // SECTION: No. 6 — BINARY SEARCH (Pencarian Produk)
    // ============================================================
    BinarySearch binarySearchIndex;  // Binary search engine untuk cari produk by ID

    // ============================================================
    // SECTION: No. 7 — BINARY SEARCH TREE (Urut Harga Katalog)
    // ============================================================
    PriceBST bstElektronik;   // BST untuk katalog elektronik (sorted by price)
    PriceBST bstPakaian;      // BST untuk katalog pakaian (sorted by price)
    PriceBST bstPerabotan;    // BST untuk katalog perabotan (sorted by price)

    // ============================================================
    // SECTION: No. 8 — AVL TREE (Penyimpanan Produk by ID)
    // ============================================================
    AVLTree avlTree;  // AVL tree untuk self-balancing storage produk by ID

    // ============================================================
    // SECTION: No. 9 — HASH TABLE (Quick Lookup O(1))
    // ============================================================
    ProductHashTable productHashTable;   // Hash table untuk product lookup by ID — O(1)
    VoucherHashTable voucherHashTable;   // Hash table untuk voucher lookup by code — O(1)
    OrderHashTable   orderHashTable;     // Hash table untuk order lookup by order ID — O(1)

    // ============================================================
    // SECTION: Helper Functions — Refresh & Routing Display
    // ============================================================
    void refreshTampilanKatalog();    // Helper: refresh display katalog ke UI
    void refreshTampilanKeranjang();  // Helper: refresh display keranjang ke UI
    void refreshTampilanAntrean();    // Helper: refresh display antrean order ke UI
    void refreshDashboardAdmin();     // Helper: refresh dashboard admin display
    void refreshTrackingPelanggan();  // Helper: refresh tracking display untuk customer

    // Helper routing visual: dapatkan string visual rute dari asal ke tujuan
    QString getRuteVisual(const QString& asal, const QString& tujuan);

    // ============================================================
    // PAGE 0: LOGIN — Halaman Login Awal
    // ============================================================
    QWidget   *pageLogin;         // Container halaman login
    QLabel    *lblLoginTitle;     // Label judul "Login"
    QLabel    *lblLoginUser;      // Label "Username:"
    QLineEdit *txtLoginUser;      // Input field username
    QLabel    *lblLoginPass;      // Label "Password:"
    QLineEdit *txtLoginPass;      // Input field password
    QPushButton *btnLoginMasuk;   // Tombol "Masuk"
    QLabel    *lblLoginInfo;      // Label untuk info/error message login

    // ============================================================
    // SHELL UTAMA — Container setelah login (Sidebar + Stacked Pages)
    // ============================================================
    QWidget      *shellWidget;       // Container utama setelah login
    QHBoxLayout  *shellLayout;       // Horizontal layout: sidebar + stacked widget

    // --- SIDEBAR NAVIGATION ---
    QFrame  *sidebarFrame;           // Frame sidebar untuk menu buttons
    QLabel  *lblMenu;                // Label "MENU"
    QLabel  *lblWelcome;             // Label ucapan welcome + username
    QPushButton *btnKatalog;         // Tombol navigasi ke halaman katalog
    QPushButton *btnKeranjang;       // Tombol navigasi ke halaman keranjang
    QPushButton *btnAntrean;         // Tombol halaman antrean (ADMIN ONLY)
    QPushButton *btnRute;            // Tombol halaman rute/graph (ADMIN ONLY)
    QPushButton *btnTrackPelanggan;  // Tombol tracking order (CUSTOMER ONLY)
    QPushButton *btnAdminDashboard;  // Tombol dashboard admin (ADMIN ONLY)
    QPushButton *btnManajemenProduk; // Tombol manajemen produk (ADMIN ONLY)
    QPushButton *btnHashLookup;      // Tombol quick lookup hash table (ADMIN ONLY)
    QPushButton *btnLogout;          // Tombol logout
    QPushButton *btnBack;            // Tombol navigasi back ke halaman sebelumnya (Stack)

    QStackedWidget *stackedWidget;   // Stacked widget untuk multiple pages

    // ============================================================
    // PAGE 0: KATALOG — Halaman Katalog Produk
    // ============================================================
    QWidget     *pageKatalog;           // Container halaman katalog
    QLabel      *lblPromoBanner;        // Label promo banner (rotating)
    QPushButton *btnKatElektronik;      // Tombol filter kategori Elektronik
    QPushButton *btnKatPakaian;         // Tombol filter kategori Pakaian
    QPushButton *btnKatPerabotan;       // Tombol filter kategori Perabotan
    QGroupBox   *groupProduk;           // Group box menampilkan detail produk current
    QLabel      *lblProductIdVal;       // Label value ID produk
    QLabel      *lblProductNameVal;     // Label value nama produk
    QLabel      *lblProductCategoryVal; // Label value kategori produk
    QLabel      *lblProductPriceVal;    // Label value harga produk
    QLabel      *lblPotonganDiskon;     // Label potongan/diskon dari voucher
    QPushButton *btnPrevCatalog;        // Tombol navigasi produk sebelumnya (prev)
    QPushButton *btnNextCatalog;        // Tombol navigasi produk berikutnya (next)
    QPushButton *btnAddToCart;          // Tombol tambah produk ke keranjang
    QPushButton *btnUrutHarga;          // Tombol sort harga (BST)
    QPushButton *btnSearchId;           // Tombol cari by ID (Binary Search)

    // ============================================================
    // PAGE 1: KERANJANG — Halaman Keranjang Belanja
    // ============================================================
    QWidget     *pageKeranjang;         // Container halaman keranjang
    QLabel      *lblCartTitle;          // Label "Keranjang Belanja"
    QListWidget *listCart;              // List widget menampilkan item keranjang
    QLineEdit   *txtKodeDiskon;         // Input field kode voucher/diskon
    QPushButton *btnPakaiDiskon;        // Tombol apply diskon
    QLabel      *lblTotalBelanja;       // Label total harga belanja
    QComboBox   *comboWilayahHub;       // Dropdown pilih wilayah/hub
    QLabel      *lblHasilBFS;           // Label menampilkan hasil BFS (rute)
    QPushButton *btnHapusCart;          // Tombol hapus item dari keranjang
    QPushButton *btnCheckout;           // Tombol checkout order
    QLabel      *lblOpsiKirimTitle;     // Label "Opsi Pengiriman"
    QComboBox   *comboPengiriman;       // Dropdown pengiriman (fallback untuk single package)
    QPushButton *btnAturLayanan;        // Tombol buka dialog pilih layanan per sub-paket
    QLabel      *lblRingkasanLayanan;   // Label ringkasan pilihan layanan yang sudah dipilih
    // Map menyimpan pilihan instan/reguler per gudang asal (key = nama gudang)
    std::map<std::string, bool> pilihanInstanPerGudang;

    // ============================================================
    // PAGE 2: ANTREAN — Halaman Antrean Order (ADMIN ONLY)
    // ============================================================
    QWidget     *pageAntrean;           // Container halaman antrean
    QLabel      *lblQueueTitle;         // Label "Antrean Order"
    QListWidget *listQueue;             // List widget menampilkan daftar order dalam antrean
    QTextEdit   *txtDetailAntrean;      // Text editor menampilkan detail order di depan antrean
    QPushButton *btnProsesOrder;        // Tombol proses order (dequeue dari antrean)

    // ============================================================
    // PAGE 3: RUTE — Halaman Rute/Graph (ADMIN ONLY)
    // ============================================================
    QWidget     *pageRute;              // Container halaman rute
    QLabel      *lblGraphTitle;         // Label "Peta Logistik"
    QTextEdit   *txtGraphNetwork;       // Text editor menampilkan seluruh jaringan graph
    QLabel      *lblHasilDFS;           // Label menampilkan hasil DFS (audit konektivitas)

    // ============================================================
    // PAGE 4: DASHBOARD ADMIN — Halaman Dashboard Admin (ADMIN ONLY)
    // ============================================================
    QWidget     *pageAdminDashboard;    // Container halaman dashboard admin
    QLabel      *lblAdminTitle;         // Label "Dashboard Admin"
    QListWidget *listAdminOrders;       // List widget menampilkan daftar order
    QTextEdit   *txtAdminDetail;        // Text editor menampilkan detail order terpilih
    QComboBox   *comboAdminStatus;      // Dropdown untuk memilih status baru order
    QPushButton *btnAdminUpdate;        // Tombol update status order

    // ============================================================
    // PAGE 5: TRACKING PELANGGAN — Halaman Tracking (CUSTOMER ONLY)
    // ============================================================
    QWidget     *pageTrackPelanggan;    // Container halaman tracking
    QLabel      *lblTrackTitle;         // Label "Tracking Pesanan"
    QListWidget *listTrackOrders;       // List widget menampilkan daftar order pelanggan
    QTextEdit   *txtTrackDetail;        // Text editor menampilkan detail tracking order
    QPushButton *btnRefreshTrack;       // Tombol refresh data tracking

    // ============================================================
    // PAGE 6: MANAJEMEN PRODUK — Halaman Manajemen Produk (ADMIN ONLY)
    // ============================================================
    QWidget      *pageManajemenProduk;  // Container halaman manajemen produk
    QLabel       *lblManajemenTitle;    // Label "Manajemen Produk"
    QTableWidget *tabelProdukAdmin;     // Table widget menampilkan daftar semua produk
    // Form input untuk CRUD produk
    QLineEdit    *txtProdukId;          // Input field ID produk
    QLineEdit    *txtProdukNama;        // Input field nama produk
    QComboBox    *comboProdukKategori;  // Dropdown kategori produk
    QLineEdit    *txtProdukHarga;       // Input field harga produk
    QComboBox    *comboProdukHub;       // Dropdown hub/asal produk
    QPushButton  *btnProdukTambah;      // Tombol tambah produk baru
    QPushButton  *btnProdukUpdate;      // Tombol update produk terpilih
    QPushButton  *btnProdukHapus;       // Tombol hapus produk terpilih
    QPushButton  *btnProdukBersihkan;   // Tombol bersihkan form input

    // ============================================================
    // PAGE 7: HASH TABLE QUICK LOOKUP — Halaman Lookup (ADMIN ONLY)
    // ============================================================
    QWidget      *pageHashLookup;       // Container halaman hash table lookup
    QLabel       *lblHashTitle;         // Label "Quick Lookup Hash Table"
    // Tab 1: Product Lookup
    QLineEdit    *txtHashProdukId;      // Input field product ID untuk lookup
    QPushButton  *btnHashCariProduk;    // Tombol cari product by ID di hash table
    QTextEdit    *txtHashProdukResult;  // Text editor menampilkan hasil lookup product
    
    // Tab 2: Voucher Lookup
    QLineEdit    *txtHashVoucherKode;   // Input field kode voucher untuk lookup
    QPushButton  *btnHashCariVoucher;   // Tombol cari voucher by code di hash table
    QTextEdit    *txtHashVoucherResult; // Text editor menampilkan hasil lookup voucher
    
    // Tab 3: Order Lookup
    QLineEdit    *txtHashOrderId;       // Input field order ID untuk lookup
    QPushButton  *btnHashCariOrder;     // Tombol cari order by ID di hash table
    QTextEdit    *txtHashOrderResult;   // Text editor menampilkan hasil lookup order
    
    // Stats panel
    QTextEdit    *txtHashStats;         // Text editor menampilkan statistik hash table

    // ============================================================
    // ROOT STACKED WIDGET — Switch antara Login & Shell
    // ============================================================
    QStackedWidget *rootStack;  // Stacked widget utama: index 0 = login page, index 1 = shell (setelah login)
};

#endif // MAINWINDOW_H