#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QGroupBox>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTime>
#include <QTimer>
#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <queue>   // FIX: diperlukan untuk runBFS (std::queue)
#include <stack>   // FIX: diperlukan untuk runDFS (std::stack)
#include <set>

// =========================================================
// INCLUDE STRUKTUR DATA TAMBAHAN
// =========================================================
#include "nav_stack.h"      // No.4: Stack — Riwayat Navigasi
#include "binary_search.h"  // No.6: Binary Search — Cari Produk
#include "bst.h"            // No.7: BST — Urut Harga Katalog
#include "avl_tree.h"       // No.8: AVL Tree — Penyimpanan Produk by ID
#include "hash_table.h"     // No.9: Hash Table — Quick Lookup O(1)

// =========================================================
// STRUKTUR DATA: CIRCULAR LINKED LIST (Banner Promo)
// =========================================================
struct PromoNode {
    std::string teks;
    PromoNode* next = nullptr;
};

class PromoBanner {
private:
    PromoNode* head    = nullptr;
    PromoNode* current = nullptr;
    int total          = 0;

public:
    ~PromoBanner() {
        if (!head) return;
        // Putus siklus dulu sebelum delete agar tidak infinite loop
        PromoNode* temp = head;
        do {
            PromoNode* next = temp->next;
            delete temp;
            temp = next;
        } while (temp != head);
    }

    void addPromo(const std::string& teks) {
        PromoNode* node = new PromoNode{teks, nullptr};
        if (!head) {
            head          = node;
            node->next    = head;   // satu node menunjuk ke dirinya sendiri (circular)
            current       = head;
        } else {
            // Cari node terakhir (yang next-nya = head)
            PromoNode* tail = head;
            while (tail->next != head) tail = tail->next;
            tail->next = node;
            node->next = head;      // sambungkan kembali ke head → circular
        }
        total++;
    }

    // Maju ke node berikutnya; karena circular, setelah node terakhir otomatis kembali ke head
    void moveNext() {
        if (current) current = current->next;
    }

    std::string getTeks() const {
        return current ? current->teks : "";
    }

    bool isEmpty() const { return head == nullptr; }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// =========================================================
// ENUM ROLE LOGIN
// =========================================================
enum class UserRole { NONE, PELANGGAN, ADMIN };

// =========================================================
// STRUKTUR DATA: DOUBLE & SINGLE LINKED LIST
// =========================================================
struct ProductNode {
    int id;
    std::string name;
    std::string category;
    long price;
    std::string origin;
    ProductNode* next = nullptr;
    ProductNode* prev = nullptr;
};

class CatalogDLL {
private:
    ProductNode* head = nullptr;
    ProductNode* tail = nullptr;
    ProductNode* current = nullptr;

public:
    CatalogDLL() : head(nullptr), tail(nullptr), current(nullptr) {}
    ~CatalogDLL() {
        ProductNode* temp = head;
        while (temp != nullptr) {
            ProductNode* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
    }

    void insertProduct(int id, std::string name, std::string cat, long price, std::string orig) {
        ProductNode* newNode = new ProductNode{id, name, cat, price, orig, nullptr, nullptr};
        if (head == nullptr) { head = tail = current = newNode; }
        else { tail->next = newNode; newNode->prev = tail; tail = newNode; }
    }

    ProductNode* getCurrentNode() { return current; }
    void moveNext() { if (current && current->next) current = current->next; }
    void movePrev() { if (current && current->prev) current = current->prev; }

    std::vector<ProductNode*> getSortedByPrice(bool asc) {
        std::vector<ProductNode*> list;
        ProductNode* temp = head;
        while (temp != nullptr) { list.push_back(temp); temp = temp->next; }
        std::sort(list.begin(), list.end(), [asc](ProductNode* a, ProductNode* b) {
            return asc ? (a->price < b->price) : (a->price > b->price);
        });
        return list;
    }

    std::vector<ProductNode*> searchProductBySubstring(std::string kw) {
        std::vector<ProductNode*> res;
        std::string lowerKw = kw;
        std::transform(lowerKw.begin(), lowerKw.end(), lowerKw.begin(), ::tolower);
        ProductNode* temp = head;
        while (temp != nullptr) {
            std::string lowerName = temp->name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            if (lowerName.find(lowerKw) != std::string::npos) res.push_back(temp);
            temp = temp->next;
        }
        return res;
    }

    // Hapus semua node dan reset pointer (diperlukan saat rebuild katalog)
    void clearCatalog() {
        while (head) {
            ProductNode* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = current = nullptr;
    }
};

class CartSLL {
private:
    ProductNode* head = nullptr;
public:
    CartSLL() : head(nullptr) {}
    ~CartSLL() { clearList(); }

    void insertItem(int id, std::string name, long price, std::string orig) {
        ProductNode* newNode = new ProductNode{id, name, "", price, orig, nullptr, nullptr};
        if (!head) { head = newNode; }
        else { ProductNode* temp = head; while (temp->next) temp = temp->next; temp->next = newNode; }
    }

    void deleteNodeAt(int idx) {
        if (!head) return;
        ProductNode* temp = head;
        if (idx == 0) { head = head->next; delete temp; return; }
        for (int i = 0; temp != nullptr && i < idx - 1; i++) temp = temp->next;
        if (!temp || !temp->next) return;
        ProductNode* next = temp->next->next;
        delete temp->next;
        temp->next = next;
    }

    long calculateTotal() {
        long total = 0;
        ProductNode* temp = head;
        while (temp) { total += temp->price; temp = temp->next; }
        return total;
    }

    bool isEmpty() { return head == nullptr; }

    void clearList() {
        while (head) { ProductNode* temp = head; head = head->next; delete temp; }
    }

    std::vector<std::string> getAllItemsString() {
        std::vector<std::string> res;
        ProductNode* temp = head;
        while (temp) { res.push_back(temp->name + " (Rp " + std::to_string(temp->price) + ")"); temp = temp->next; }
        return res;
    }

    std::vector<std::string> getAllOrigins() {
        std::vector<std::string> res;
        ProductNode* temp = head;
        while (temp) { res.push_back(temp->origin); temp = temp->next; }
        return res;
    }

    std::vector<std::string> getAllNames() {
        std::vector<std::string> res;
        ProductNode* temp = head;
        while (temp) { res.push_back(temp->name); temp = temp->next; }
        return res;
    }

    std::vector<long> getAllPrices() {
        std::vector<long> res;
        ProductNode* temp = head;
        while (temp) { res.push_back(temp->price); temp = temp->next; }
        return res;
    }
};

// =========================================================
// STRUKTUR DATA: QUEUE & GRAPH
// =========================================================
struct QueueNode {
    std::string idPelanggan;
    long totalBayar;
    std::string detailNota;
    bool isInstant;
    std::string kotaTujuan;
    std::string kotaAsal;
    std::string namaPelanggan;
    QueueNode* next = nullptr;
};

class LogisticQueue {
private:
    QueueNode* front = nullptr;
    QueueNode* rear  = nullptr;

public:
    LogisticQueue() : front(nullptr), rear(nullptr) {}
    ~LogisticQueue() { while (!isEmpty()) dequeue(); }

    void enqueueOrderPriority(std::string id, long total, std::string detail,
                              bool instant, std::string dest, std::string asal, std::string pemilik) {
        QueueNode* newNode = new QueueNode{id, total, detail, instant, dest, asal, pemilik, nullptr};
        if (isEmpty()) { front = rear = newNode; return; }
        if (instant) {
            if (!front->isInstant) { newNode->next = front; front = newNode; }
            else {
                QueueNode* temp = front;
                while (temp->next != nullptr && temp->next->isInstant) temp = temp->next;
                newNode->next = temp->next;
                temp->next = newNode;
                if (newNode->next == nullptr) rear = newNode;
            }
        } else { rear->next = newNode; rear = newNode; }
    }

    void dequeue() {
        if (isEmpty()) return;
        QueueNode* temp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
    }

    bool isEmpty() { return front == nullptr; }
    std::string peekFrontDetail()      { return isEmpty() ? "" : front->detailNota; }
    std::string peekFrontDestination() { return isEmpty() ? "" : front->kotaTujuan; }
    std::string peekFrontOrigin()      { return isEmpty() ? "Hub Jakarta" : front->kotaAsal; }
    std::string peekFrontOwner()       { return isEmpty() ? "" : front->namaPelanggan; }
    std::string peekFrontId()          { return isEmpty() ? "" : front->idPelanggan; }

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

class LogisticGraph {
private:
    std::vector<std::string> vertices;
    std::vector<std::vector<std::pair<int,int>>> adjList;

    int getVertexIndex(std::string name) {
        for (size_t i = 0; i < vertices.size(); ++i)
            if (vertices[i] == name) return static_cast<int>(i);
        vertices.push_back(name);
        adjList.push_back(std::vector<std::pair<int,int>>());
        return static_cast<int>(vertices.size() - 1);
    }

public:
    void addRoute(std::string u, std::string v, int w) {
        int uIdx = getVertexIndex(u);
        int vIdx = getVertexIndex(v);
        adjList[uIdx].push_back({vIdx, w});
    }

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
    // FIX: BFS — Breadth-First Search (Queue-based)
    // Menemukan rute dengan jumlah hop paling sedikit
    // dari 'start' ke 'end', lalu tampilkan jalur + total waktu
    // -------------------------------------------------------
    std::string runBFS(const std::string& start, const std::string& end) {
        if (vertices.empty()) return "❌ Graf kosong.";

        int startIdx = -1, endIdx = -1;
        for (int i = 0; i < (int)vertices.size(); ++i) {
            if (vertices[i] == start) startIdx = i;
            if (vertices[i] == end)   endIdx   = i;
        }
        if (startIdx < 0) return "❌ Vertex awal tidak ditemukan: " + start;
        if (endIdx   < 0) return "❌ Vertex tujuan tidak ditemukan: " + end;
        if (startIdx == endIdx) return "✅ Sudah di lokasi tujuan: " + start;

        // BFS menggunakan std::queue (FIFO)
        std::vector<int>  parent(vertices.size(), -1);
        std::vector<int>  cost(vertices.size(), 0);
        std::vector<bool> visited(vertices.size(), false);
        std::queue<int>   q;

        q.push(startIdx);
        visited[startIdx] = true;
        bool found = false;

        while (!q.empty()) {
            int curr = q.front(); q.pop();
            if (curr == endIdx) { found = true; break; }
            for (auto& edge : adjList[curr]) {
                if (!visited[edge.first]) {
                    visited[edge.first] = true;
                    parent[edge.first]  = curr;
                    cost[edge.first]    = cost[curr] + edge.second;
                    q.push(edge.first);
                }
            }
        }

        if (!found) return "❌ Tidak ada rute dari " + start + " ke " + end;

        // Rekonstruksi jalur dari end ke start (lalu balik)
        std::vector<std::string> path;
        for (int v = endIdx; v != -1; v = parent[v])
            path.push_back(vertices[v]);
        std::reverse(path.begin(), path.end());

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
    // FIX: DFS — Depth-First Search (Stack-based)
    // Audit konektivitas: jelajahi semua hub yang bisa dicapai
    // dari 'start', menggunakan Stack eksplisit (bukan rekursi)
    // -------------------------------------------------------
    std::string runDFS(const std::string& start) {
        if (vertices.empty()) return "❌ Graf kosong.";

        int startIdx = -1;
        for (int i = 0; i < (int)vertices.size(); ++i)
            if (vertices[i] == start) { startIdx = i; break; }
        if (startIdx < 0) return "❌ Vertex awal tidak ditemukan: " + start;

        // DFS menggunakan std::stack (LIFO)
        std::vector<bool>        visited(vertices.size(), false);
        std::vector<std::string> order;
        std::stack<int>          s;

        s.push(startIdx);
        while (!s.empty()) {
            int curr = s.top(); s.pop();
            if (visited[curr]) continue;
            visited[curr] = true;
            order.push_back(vertices[curr]);
            // Push tetangga ke stack (urutan terbalik agar kiri diproses dulu)
            for (int i = (int)adjList[curr].size() - 1; i >= 0; --i)
                if (!visited[adjList[curr][i].first])
                    s.push(adjList[curr][i].first);
        }

        std::string result = "=== HASIL DFS (Audit Konektivitas dari " + start + ") ===\n";
        result += "Urutan Kunjungan:\n";
        for (int i = 0; i < (int)order.size(); ++i)
            result += "  " + std::to_string(i + 1) + ". " + order[i] + "\n";
        result += "\nTotal Hub Terjangkau : " + std::to_string(order.size());

        // Periksa hub yang TIDAK terjangkau
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
// STRUKTUR ACTIVE ORDER — dengan field lastUpdatedByAdmin
// =========================================================
struct ActiveOrder {
    std::string idPelanggan;
    std::string namaPelanggan;      // username pelanggan pemilik order
    std::string detailNota;
    std::string kotaAsal;
    std::string kotaTujuan;
    bool isInstant;
    std::string statusLogistik;
    int sisaWaktuJam;
    std::string lokasiSekarang;     // hub terakhir yang sudah dicapai paket
    std::string lastAdminUpdate;    // timestamp terakhir admin update
    bool sudahDiUpdateAdmin;        // flag: hanya tampil ke pelanggan setelah admin update
};

// =========================================================
// KELAS UTAMA MAINWINDOW
// =========================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Login
    void klikLogin();
    void klikLogout();

    // Navigasi pelanggan
    void navigasiHalaman();
    void gantiKategoriKatalog();
    void klikNextProduk();
    void klikPrevProduk();
    void klikTambahKeranjang();
    void klikHapusCart();
    void klikCheckout();
    void klikAturLayananPerPaket();
    void klikUrutHarga();
    void klikCariId();
    void klikBack();                    // Stack: tombol kembali halaman sebelumnya

    // Pelacakan pelanggan
    void klikRefreshTrackingPelanggan();

    // Promo banner (Circular Linked List)
    void rotasiPromoBanner();

    // Admin
    void navigasiAdmin();
    void klikProsesOrderAdmin();
    void klikUpdateStatusAdmin();

    // Admin — Manajemen Produk (Add / Update / Delete)
    void klikTambahProdukAdmin();
    void klikUpdateProdukAdmin();
    void klikHapusProdukAdmin();
    void refreshTabelProdukAdmin();

    // Hash Table — Quick Lookup
    void klikQuickLookup();

private:
    Ui::MainWindow *ui;

    // ---- State login ----
    UserRole roleAktif = UserRole::NONE;
    std::string usernameAktif = "";

    // ---- Data bersama ----
    std::vector<ActiveOrder> databasePelacakan;

    // ---- Circular Linked List: Banner Promo ----
    PromoBanner promoBanner;
    QTimer     *promoTimer = nullptr;

    // ---- Data katalog & keranjang ----
    std::string kategoriAktif;
    CatalogDLL katalogElektronikDLL;
    CatalogDLL katalogPakaianDLL;
    CatalogDLL katalogPerabotanDLL;
    CartSLL keranjangSLL;
    LogisticQueue antreanQueue;
    LogisticGraph petaLogistik;

    // ---- No.4: Stack — Riwayat Navigasi Halaman ----
    NavStack navStack;

    // ---- No.6: Binary Search — Pencarian Produk ----
    BinarySearch binarySearchIndex;

    // ---- No.7: BST — Urut Harga Katalog ----
    PriceBST bstElektronik;
    PriceBST bstPakaian;
    PriceBST bstPerabotan;

    // ---- No.8: AVL Tree — Penyimpanan Produk by ID ----
    AVLTree avlTree;

    // ---- No.9: Hash Table — Quick Lookup O(1) ----
    ProductHashTable productHashTable;   // 1. Product Lookup by ID
    VoucherHashTable voucherHashTable;   // 2. Voucher Code Lookup
    OrderHashTable   orderHashTable;     // 3. Order Lookup by Order ID

    // ---- Helper refresh ----
    void refreshTampilanKatalog();
    void refreshTampilanKeranjang();
    void refreshTampilanAntrean();
    void refreshDashboardAdmin();
    void refreshTrackingPelanggan();

    // ---- Helper routing visual ----
    QString getRuteVisual(const QString& asal, const QString& tujuan);

    // =========================================================
    // HALAMAN LOGIN
    // =========================================================
    QWidget   *pageLogin;
    QLabel    *lblLoginTitle;
    QLabel    *lblLoginUser;
    QLineEdit *txtLoginUser;
    QLabel    *lblLoginPass;
    QLineEdit *txtLoginPass;
    QPushButton *btnLoginMasuk;
    QLabel    *lblLoginInfo;

    // =========================================================
    // SHELL UTAMA (setelah login)
    // =========================================================
    QWidget      *shellWidget;      // container sesudah login
    QHBoxLayout  *shellLayout;

    // --- Sidebar ---
    QFrame  *sidebarFrame;
    QLabel  *lblMenu;
    QLabel  *lblWelcome;
    QPushButton *btnKatalog;
    QPushButton *btnKeranjang;
    QPushButton *btnAntrean;        // hanya admin
    QPushButton *btnRute;           // hanya admin
    QPushButton *btnTrackPelanggan; // hanya pelanggan
    QPushButton *btnAdminDashboard; // hanya admin
    QPushButton *btnManajemenProduk;// hanya admin — kelola produk
    QPushButton *btnHashLookup;     // hanya admin — quick lookup hash table
    QPushButton *btnLogout;
    QPushButton *btnBack;           // Stack: tombol navigasi Back

    QStackedWidget *stackedWidget;

    // =========================================================
    // PAGE 0 — KATALOG
    // =========================================================
    QWidget     *pageKatalog;
    QLabel      *lblPromoBanner;
    QPushButton *btnKatElektronik;
    QPushButton *btnKatPakaian;
    QPushButton *btnKatPerabotan;
    QGroupBox   *groupProduk;
    QLabel      *lblProductIdVal;
    QLabel      *lblProductNameVal;
    QLabel      *lblProductCategoryVal;
    QLabel      *lblProductPriceVal;
    QLabel      *lblPotonganDiskon;
    QPushButton *btnPrevCatalog;
    QPushButton *btnNextCatalog;
    QPushButton *btnAddToCart;
    QPushButton *btnUrutHarga;
    QPushButton *btnSearchId;

    // =========================================================
    // PAGE 1 — KERANJANG
    // =========================================================
    QWidget     *pageKeranjang;
    QLabel      *lblCartTitle;
    QListWidget *listCart;
    QLineEdit   *txtKodeDiskon;
    QPushButton *btnPakaiDiskon;
    QLabel      *lblTotalBelanja;
    QComboBox   *comboWilayahHub;
    QLabel      *lblHasilBFS;
    QPushButton *btnHapusCart;
    QPushButton *btnCheckout;
    QLabel      *lblOpsiKirimTitle;
    QComboBox   *comboPengiriman;          // kept for single-package fallback
    QPushButton *btnAturLayanan;           // buka dialog pilih layanan per sub-paket
    QLabel      *lblRingkasanLayanan;      // ringkasan pilihan yang sudah dipilih
    // Menyimpan pilihan instan/reguler per gudang asal (key = nama gudang)
    std::map<std::string, bool> pilihanInstanPerGudang;

    // =========================================================
    // PAGE 2 — ANTREAN (ADMIN ONLY)
    // =========================================================
    QWidget     *pageAntrean;
    QLabel      *lblQueueTitle;
    QListWidget *listQueue;
    QTextEdit   *txtDetailAntrean;
    QPushButton *btnProsesOrder;

    // =========================================================
    // PAGE 3 — RUTE / GRAPH (ADMIN ONLY)
    // =========================================================
    QWidget     *pageRute;
    QLabel      *lblGraphTitle;
    QTextEdit   *txtGraphNetwork;
    QLabel      *lblHasilDFS;

    // =========================================================
    // PAGE 4 — DASHBOARD ADMIN
    // =========================================================
    QWidget     *pageAdminDashboard;
    QLabel      *lblAdminTitle;
    QListWidget *listAdminOrders;
    QTextEdit   *txtAdminDetail;
    QComboBox   *comboAdminStatus;
    QPushButton *btnAdminUpdate;

    // =========================================================
    // PAGE 5 — TRACKING PELANGGAN
    // =========================================================
    QWidget     *pageTrackPelanggan;
    QLabel      *lblTrackTitle;
    QListWidget *listTrackOrders;
    QTextEdit   *txtTrackDetail;
    QPushButton *btnRefreshTrack;

    // =========================================================
    // PAGE 6 — MANAJEMEN PRODUK (ADMIN ONLY)
    // =========================================================
    QWidget      *pageManajemenProduk;
    QLabel       *lblManajemenTitle;
    QTableWidget *tabelProdukAdmin;   // daftar semua produk
    // Form input
    QLineEdit    *txtProdukId;
    QLineEdit    *txtProdukNama;
    QComboBox    *comboProdukKategori;
    QLineEdit    *txtProdukHarga;
    QComboBox    *comboProdukHub;
    QPushButton  *btnProdukTambah;
    QPushButton  *btnProdukUpdate;
    QPushButton  *btnProdukHapus;
    QPushButton  *btnProdukBersihkan;

    // =========================================================
    // PAGE 7 — HASH TABLE QUICK LOOKUP (ADMIN ONLY)
    // =========================================================
    QWidget      *pageHashLookup;
    QLabel       *lblHashTitle;
    // Tab 1: Product Lookup
    QLineEdit    *txtHashProdukId;
    QPushButton  *btnHashCariProduk;
    QTextEdit    *txtHashProdukResult;
    // Tab 2: Voucher Lookup
    QLineEdit    *txtHashVoucherKode;
    QPushButton  *btnHashCariVoucher;
    QTextEdit    *txtHashVoucherResult;
    // Tab 3: Order Lookup
    QLineEdit    *txtHashOrderId;
    QPushButton  *btnHashCariOrder;
    QTextEdit    *txtHashOrderResult;
    // Stats panel
    QTextEdit    *txtHashStats;

    // =========================================================
    // ROOT STACKED (login vs shell)
    // =========================================================
    QStackedWidget *rootStack;  // index 0 = login, index 1 = shell
};

#endif // MAINWINDOW_H