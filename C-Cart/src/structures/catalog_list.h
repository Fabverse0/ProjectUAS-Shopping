#ifndef CATALOG_LIST_H
#define CATALOG_LIST_H

#include "../models/product.h"

/**
 * @brief Node untuk Double Linked List katalog produk
 */
struct CatalogNode {
    Product product;
    CatalogNode* prev;
    CatalogNode* next;

    CatalogNode(Product p) : product(p), prev(nullptr), next(nullptr) {}
};

/**
 * @brief Double Linked List untuk navigasi katalog produk
 * Tombol Prev/Next untuk browse produk tanpa akses ulang seluruh list
 */
class CatalogList {
private:
    CatalogNode* head;
    CatalogNode* tail;
    CatalogNode* current;   // posisi user sekarang
    int size;

public:
    CatalogList();
    ~CatalogList();

    // Operasi
    void tambahProduk(Product p);   // insert di belakang
    void next();                    // pindah ke produk berikutnya
    void prev();                    // pindah ke produk sebelumnya
    void resetKeCurrent();          // kembali ke awal

    // Tampilan
    void tampilkanCurrent() const;  // tampilkan produk sekarang
    void tampilkanSemua() const;    // tampilkan semua produk

    // Getter
    bool isEmpty() const;
    int getSize() const;
    CatalogNode* getCurrent() const;
    Product getCurrentProduct() const;
};

#endif // CATALOG_LIST_H