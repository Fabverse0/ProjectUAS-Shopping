#ifndef CATALOG_LIST_H
#define CATALOG_LIST_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

// 1. STRUKTUR DATA UNTUK PRODUK KATALOG (DOUBLE LINKED LIST)
struct ProductNode {
    int id;
    std::string name;
    std::string category;
    long price;
    std::string origin; // Lokasi asal gudang toko/hub logistik
    int stock;          // 🌟 FITUR BARU: Jumlah stok barang yang tersedia
    ProductNode* next;
    ProductNode* prev;
};

class CatalogList {
private:
    ProductNode* head;
    ProductNode* tail;
    ProductNode* current;
    int totalProducts;   // Menyimpan jumlah total variasi produk di katalog

public:
    CatalogList() : head(nullptr), tail(nullptr), current(nullptr), totalProducts(0) {}

    // Destructor untuk membersihkan memori agar tidak terjadi Memory Leak
    ~CatalogList() {
        clearCatalog();
    }

    // Fungsi memeriksa apakah katalog kosong
    bool isEmpty() const { return head == nullptr; }

    // Mendapatkan total variasi produk saat ini
    int size() const { return totalProducts; }

    // 🌟 INSERT PRODUCT (Dengan validasi stok awal)
    void insertProduct(int id, std::string name, std::string category, long price, std::string origin, int initialStock = 10) {
        ProductNode* newNode = new ProductNode{id, name, category, price, origin, initialStock, nullptr, nullptr};
        if (!head) {
            head = tail = current = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        totalProducts++;
    }

    // 🌟 MANAJEMEN STOK: Mengurangi stok jika sukses dibeli/masuk keranjang
    bool reduceStock(int id, int quantity) {
        ProductNode* temp = head;
        while (temp) {
            if (temp->id == id) {
                if (temp->stock >= quantity) {
                    temp->stock -= quantity;
                    return true; // Stok berhasil dikurangi
                }
                return false; // Stok tidak mencukupi
            }
            temp = temp->next;
        }
        return false; // Produk tidak ditemukan
    }

    // 🌟 MANAJEMEN STOK: Mengembalikan stok jika item di keranjang dihapus/batal
    void restockProduct(int id, int quantity) {
        ProductNode* temp = head;
        while (temp) {
            if (temp->id == id) {
                temp->stock += quantity;
                return;
            }
            temp = temp->next;
        }
    }

    // Mendapatkan pointer ke node aktif saat ini (untuk navigasi GUI)
    ProductNode* getCurrentNode() { return current; }

    // Mengatur ulang pointer navigasi ke awal katalog
    void resetNavigation() { current = head; }

    // Navigasi Maju (Next)
    void moveNext() {
        if (current && current->next) current = current->next;
    }

    // Navigasi Mundur (Previous)
    void movePrev() {
        if (current && current->prev) current = current->prev;
    }

    // 🌟 SORTING DENGAN ALGORITMA YANG LEBIH CEPAT (std::sort - O(N log N))
    std::vector<ProductNode*> getSortedByPrice(bool ascending) {
        std::vector<ProductNode*> list;
        ProductNode* temp = head;
        while (temp) {
            list.push_back(temp);
            temp = temp->next;
        }

        // Menggunakan introsort bawaan C++ yang jauh lebih cepat daripada Bubble Sort murni
        std::sort(list.begin(), list.end(), [ascending](ProductNode* a, ProductNode* b) {
            return ascending ? (a->price < b->price) : (a->price > b->price);
        });

        return list;
    }

    // 🌟 PENCARIAN GLOBAL BERDASARKAN SUBSTRING NAMA (Case-Insensitive)
    std::vector<ProductNode*> searchProductBySubstring(std::string keyword) {
        std::vector<ProductNode*> result;

        // Ubah keyword ke huruf kecil agar pencarian tidak sensitif huruf besar/kecil
        std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

        ProductNode* temp = head;
        while (temp) {
            std::string tempName = temp->name;
            std::transform(tempName.begin(), tempName.end(), tempName.begin(), ::tolower);

            if (tempName.find(keyword) != std::string::npos) {
                result.push_back(temp);
            }
            temp = temp->next;
        }
        return result;
    }

    // 🌟 PENCARIAN SPESIFIK BERDASARKAN ID PRODUK
    ProductNode* findProductById(int id) {
        ProductNode* temp = head;
        while (temp) {
            if (temp->id == id) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // Fungsi membersihkan seluruh isi memori linked list
    void clearCatalog() {
        while (head) {
            ProductNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = current = nullptr;
        totalProducts = 0;
    }
};

#endif // CATALOG_LIST_H