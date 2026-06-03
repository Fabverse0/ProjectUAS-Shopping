#ifndef CART_LIST_H
#define CART_LIST_H

#include "../models/product.h"

/**
 * @brief Node untuk Single Linked List keranjang belanja
 */
struct CartNode {
    Product product;
    int qty;
    CartNode* next;

    CartNode(Product p, int qty = 1) : product(p), qty(qty), next(nullptr) {}
};

/**
 * @brief Single Linked List untuk keranjang belanja
 * Insert/delete dinamis saat tambah atau hapus barang
 */
class CartList {
private:
    CartNode* head;
    int size;

public:
    CartList();
    ~CartList();

    // Operasi utama
    void tambahProduk(Product p, int qty = 1);   // insert node
    void hapusProduk(int productId);              // delete node
    void updateQty(int productId, int qty);       // update quantity
    void kosongkan();                             // hapus semua

    // Tampilan
    void tampilkan() const;
    double totalHarga() const;                    // traversal semua node

    // Getter
    bool isEmpty() const;
    int getSize() const;
    CartNode* getHead() const;
    bool produkAda(int productId) const;
};

#endif // CART_LIST_H