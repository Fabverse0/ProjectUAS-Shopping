// Guard preprocessor untuk mencegah include ganda
#ifndef CATALOG_LIST_H
// Definisikan simbol CATALOG_LIST_H sebagai tanda file sudah di-include
#define CATALOG_LIST_H

// Include library string untuk tipe data std::string (nama, kategori, asal)
#include <string>
// Include library vector untuk menyimpan array dinamis
#include <vector>
// Include library algorithm untuk std::sort dan std::transform
#include <algorithm>
// Include library iostream untuk output (debugging)
#include <iostream>

// STRUCT: Definisikan node untuk produk katalog (DOUBLE LINKED LIST)
// Memiliki pointer next untuk traversal maju dan prev untuk traversal mundur
struct ProductNode {
    // Menyimpan ID unik produk
    int id;
    // Menyimpan nama produk
    std::string name;
    // Menyimpan kategori produk (Elektronik, Pakaian, Perabotan, dll)
    std::string category;
    // Menyimpan harga produk dalam Rupiah
    long price;
    // Menyimpan lokasi asal gudang/hub logistik tempat produk berasal
    std::string origin; // Lokasi asal gudang toko/hub logistik
    // 🌟 FITUR BARU: Menyimpan jumlah stok barang yang tersedia
    int stock;          // 🌟 FITUR BARU: Jumlah stok barang yang tersedia
    // Pointer ke node berikutnya (untuk traversal maju/next)
    ProductNode* next;
    // Pointer ke node sebelumnya (untuk traversal mundur/previous)
    ProductNode* prev;
};

// CLASS: Definisikan katalog produk menggunakan double linked list
class CatalogList {
private:
    // Pointer ke node pertama (awal) dalam katalog
    ProductNode* head;
    // Pointer ke node terakhir (akhir) dalam katalog
    ProductNode* tail;
    // Pointer ke node yang sedang aktif (untuk navigasi GUI dengan prev/next)
    ProductNode* current;
    // Menyimpan total jumlah produk yang ada di katalog
    int totalProducts;   // Menyimpan jumlah total variasi produk di katalog

public:
    // KONSTRUKTOR: Inisialisasi katalog kosong dengan semua pointer = nullptr
    CatalogList() : head(nullptr), tail(nullptr), current(nullptr), totalProducts(0) {}

    // DESTRUKTOR: Bersihkan semua node dari memori saat object dihancurkan
    ~CatalogList() {
        // Panggil clearCatalog() untuk hapus semua produk dari memori
        clearCatalog();
    }

    // FUNGSI: Cek apakah katalog kosong (tidak ada produk)
    bool isEmpty() const { return head == nullptr; }

    // FUNGSI: Dapatkan total jumlah produk yang ada di katalog
    int size() const { return totalProducts; }

    // 🌟 FUNGSI: INSERT PRODUCT ke katalog dengan validasi stok awal
    void insertProduct(int id, std::string name, std::string category, long price, std::string origin, int initialStock = 10) {
        // Buat node produk baru dengan semua data dan stok awal
        ProductNode* newNode = new ProductNode{id, name, category, price, origin, initialStock, nullptr, nullptr};
        // Jika katalog kosong (head == nullptr)
        if (!head) {
            // Set head, tail, dan current semuanya ke node baru
            head = tail = current = newNode;
        } 
        // Jika katalog tidak kosong
        else {
            // Sambungkan node baru ke akhir linked list (insert di tail)
            tail->next = newNode;
            // Set pointer prev node baru ke tail lama (untuk double-linking)
            newNode->prev = tail;
            // Update tail ke node baru
            tail = newNode;
        }
        // Increment counter totalProducts
        totalProducts++;
    }

    // 🌟 FUNGSI: MANAJEMEN STOK - Kurangi stok saat produk dibeli/masuk keranjang
    bool reduceStock(int id, int quantity) {
        // Traverse katalog dari head untuk mencari produk dengan ID yang sesuai
        ProductNode* temp = head;
        while (temp) {
            // Jika produk dengan ID ditemukan
            if (temp->id == id) {
                // Cek apakah stok cukup untuk quantity yang diminta
                if (temp->stock >= quantity) {
                    // Kurangi stok produk dengan quantity yang dibeli
                    temp->stock -= quantity;
                    // Return true menandakan pengurangan stok berhasil
                    return true; // Stok berhasil dikurangi
                }
                // Jika stok tidak mencukupi
                return false; // Stok tidak mencukupi
            }
            // Geser ke produk berikutnya
            temp = temp->next;
        }
        // Jika produk tidak ditemukan dalam katalog
        return false; // Produk tidak ditemukan
    }

    // 🌟 FUNGSI: MANAJEMEN STOK - Kembalikan stok jika item dihapus dari keranjang
    void restockProduct(int id, int quantity) {
        // Traverse katalog dari head untuk mencari produk dengan ID yang sesuai
        ProductNode* temp = head;
        while (temp) {
            // Jika produk dengan ID ditemukan
            if (temp->id == id) {
                // Tambah kembali stok produk dengan quantity yang dikembalikan
                temp->stock += quantity;
                // Exit function setelah stok dikembalikan
                return;
            }
            // Geser ke produk berikutnya
            temp = temp->next;
        }
    }

    // FUNGSI: Dapatkan pointer ke node yang sedang aktif/dipilih (untuk GUI)
    ProductNode* getCurrentNode() { return current; }

    // FUNGSI: Reset navigasi untuk mulai dari produk pertama (head)
    void resetNavigation() { current = head; }

    // FUNGSI: Navigasi maju ke produk berikutnya (Next)
    void moveNext() {
        // Jika current ada dan memiliki node berikutnya
        if (current && current->next) 
            // Update current ke node berikutnya
            current = current->next;
    }

    // FUNGSI: Navigasi mundur ke produk sebelumnya (Previous)
    void movePrev() {
        // Jika current ada dan memiliki node sebelumnya
        if (current && current->prev) 
            // Update current ke node sebelumnya
            current = current->prev;
    }

    // 🌟 FUNGSI: SORTING - Urutkan produk by harga dengan algoritma cepat (std::sort - O(N log N))
    std::vector<ProductNode*> getSortedByPrice(bool ascending) {
        // Buat vector untuk menyimpan pointer ke semua produk
        std::vector<ProductNode*> list;
        // Traverse katalog dari head untuk mengambil pointer setiap produk
        ProductNode* temp = head;
        while (temp) {
            // Tambahkan pointer produk ke vector
            list.push_back(temp);
            // Geser ke produk berikutnya
            temp = temp->next;
        }

        // SORT menggunakan std::sort (introsort) yang lebih cepat daripada bubble sort
        std::sort(list.begin(), list.end(), [ascending](ProductNode* a, ProductNode* b) {
            // Comparator: jika ascending=true urutkan dari harga kecil ke besar
            // Jika ascending=false urutkan dari harga besar ke kecil
            return ascending ? (a->price < b->price) : (a->price > b->price);
        });

        // Kembalikan vector yang sudah terurut
        return list;
    }

    // 🌟 FUNGSI: PENCARIAN GLOBAL by SUBSTRING NAMA (Case-Insensitive)
    std::vector<ProductNode*> searchProductBySubstring(std::string keyword) {
        // Buat vector untuk menyimpan hasil pencarian
        std::vector<ProductNode*> result;

        // Konversi keyword ke huruf kecil agar pencarian case-insensitive
        std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

        // Traverse katalog dari head untuk mencari produk yang cocok
        ProductNode* temp = head;
        while (temp) {
            // Copy nama produk untuk dikonversi ke huruf kecil
            std::string tempName = temp->name;
            // Konversi nama produk ke huruf kecil
            std::transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower);

            // Cek apakah keyword ada di dalam nama produk (substring match)
            if (tempName.find(keyword) != std::string::npos) {
                // Jika ditemukan, tambahkan produk ke result
                result.push_back(temp);
            }
            // Geser ke produk berikutnya
            temp = temp->next;
        }
        // Kembalikan vector berisi produk yang cocok dengan keyword
        return result;
    }

    // 🌟 FUNGSI: PENCARIAN SPESIFIK by ID PRODUK (Exact Match)
    ProductNode* findProductById(int id) {
        // Traverse katalog dari head untuk mencari produk dengan ID yang sesuai
        ProductNode* temp = head;
        while (temp) {
            // Jika produk dengan ID ditemukan
            if (temp->id == id) 
                // Kembalikan pointer ke produk yang ditemukan
                return temp;
            // Geser ke produk berikutnya
            temp = temp->next;
        }
        // Jika produk tidak ditemukan, kembalikan nullptr
        return nullptr;
    }

    // FUNGSI: Bersihkan seluruh katalog - hapus semua node dari memori
    void clearCatalog() {
        // Selama masih ada node di katalog (head != nullptr)
        while (head) {
            // Simpan pointer head saat ini
            ProductNode* temp = head;
            // Update head ke node berikutnya
            head = head->next;
            // Hapus node lama dari memori
            delete temp;
        }
        // Set tail ke nullptr karena katalog sekarang kosong
        tail = current = nullptr;
        // Reset counter totalProducts ke 0
        totalProducts = 0;
    }
};

// Akhir guard preprocessor: menutup pencegahan include ganda
#endif // CATALOG_LIST_H