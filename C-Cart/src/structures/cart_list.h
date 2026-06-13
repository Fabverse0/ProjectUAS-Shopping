#ifndef CART_LIST_H
#define CART_LIST_H

#include "../models/product.h"

// Node untuk Single Linked List keranjang belanja
struct CartNode {
    Product product;
    int qty;
    CartNode* next;

    CartNode(Product p, int qty = 1) : product(p), qty(qty), next(nullptr) {}
};

// Struktur data Single Linked List untuk mengelola keranjang belanja
class CartList {
private:
    CartNode* head;
    int size;

public:
    CartList();
    ~CartList();

    // Operasi keranjang
    void addProduct(Product p, int qty = 1);
    void removeProduct(int productId);
    void updateQty(int productId, int qty);
    void clear();

    // Tampilan dan perhitungan
    void display() const;
    double totalPrice() const;

    // Getter dan pengecekan
    bool isEmpty() const;
    int getSize() const;
    CartNode* getHead() const;
    bool hasProduct(int productId) const;
};

#endif // CART_LIST_H
