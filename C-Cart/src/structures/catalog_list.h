#ifndef CATALOG_LIST_H
#define CATALOG_LIST_H

#include "../models/product.h"

// Node untuk Double Linked List katalog produk
struct CatalogNode {
    Product product;
    CatalogNode* prev;
    CatalogNode* next;

    CatalogNode(Product p) : product(p), prev(nullptr), next(nullptr) {}
};

// Double Linked List untuk navigasi katalog produk
// Memungkinkan navigasi maju-mundur di katalog
class CatalogList {
private:
    CatalogNode* head;
    CatalogNode* tail;
    CatalogNode* current;   // Posisi saat ini
    int size;

public:
    CatalogList();
    ~CatalogList();

    // Menambah produk di akhir list
    void addProduct(Product p);

    // Navigasi
    void next();            // Pindah ke produk selanjutnya
    void prev();            // Pindah ke produk sebelumnya
    void resetToFirst();    // Kembali ke produk pertama

    // Pencarian
    Product* findById(int productId);

    // Tampilan
    void displayCurrent() const;  // Tampilkan produk aktif
    void displayAll() const;    // Tampilkan semua produk

    // Getter
    bool isEmpty() const;
    int getSize() const;
    CatalogNode* getCurrent() const;
    Product getCurrentProduct() const;
};

#endif // CATALOG_LIST_H