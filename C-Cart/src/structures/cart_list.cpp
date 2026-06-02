#include "cart_list.h"
#include <iostream>
#include <iomanip>

CartList::CartList() : head(nullptr), size(0) {}

CartList::~CartList() {
    kosongkan();
}

// ── INSERT ──────────────────────────────────────────────
void CartList::tambahProduk(Product p, int qty) {
    // Cek apakah produk sudah ada → tambah qty saja
    CartNode* curr = head;
    while (curr != nullptr) {
        if (curr->product.id == p.id) {
            curr->qty += qty;
            std::cout << "> Qty " << p.nama << " diupdate menjadi " << curr->qty << std::endl;
            return;
        }
        curr = curr->next;
    }

    // Produk belum ada → insert node baru di depan
    CartNode* newNode = new CartNode(p, qty);
    newNode->next = head;
    head = newNode;
    size++;
    std::cout << "> " << p.nama << " ditambahkan ke keranjang!" << std::endl;
}

// ── DELETE ──────────────────────────────────────────────
void CartList::hapusProduk(int productId) {
    if (head == nullptr) {
        std::cout << "Keranjang sudah kosong!" << std::endl;
        return;
    }

    // Cek node pertama
    if (head->product.id == productId) {
        CartNode* temp = head;
        head = head->next;
        std::cout << "> " << temp->product.nama << " dihapus dari keranjang." << std::endl;
        delete temp;
        size--;
        return;
    }

    // Cari di node berikutnya
    CartNode* curr = head;
    while (curr->next != nullptr) {
        if (curr->next->product.id == productId) {
            CartNode* temp = curr->next;
            curr->next = temp->next;
            std::cout << "> " << temp->product.nama << " dihapus dari keranjang." << std::endl;
            delete temp;
            size--;
            return;
        }
        curr = curr->next;
    }

    std::cout << "Produk tidak ditemukan di keranjang." << std::endl;
}

// ── UPDATE QTY ──────────────────────────────────────────
void CartList::updateQty(int productId, int qty) {
    CartNode* curr = head;
    while (curr != nullptr) {
        if (curr->product.id == productId) {
            if (qty <= 0) {
                hapusProduk(productId);
            } else {
                curr->qty = qty;
                std::cout << "> Qty diupdate menjadi " << qty << std::endl;
            }
            return;
        }
        curr = curr->next;
    }
    std::cout << "Produk tidak ditemukan di keranjang." << std::endl;
}

// ── KOSONGKAN ───────────────────────────────────────────
void CartList::kosongkan() {
    CartNode* curr = head;
    while (curr != nullptr) {
        CartNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = nullptr;
    size = 0;
}

// ── TAMPILKAN ───────────────────────────────────────────
void CartList::tampilkan() const {
    if (head == nullptr) {
        std::cout << "  Keranjang masih kosong." << std::endl;
        return;
    }

    std::cout << std::string(55, '-') << std::endl;
    std::cout << std::left
              << std::setw(4)  << "No"
              << std::setw(22) << "Nama Produk"
              << std::setw(5)  << "Qty"
              << std::setw(15) << "Harga Satuan"
              << "Subtotal" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    CartNode* curr = head;
    int no = 1;
    while (curr != nullptr) {
        double subtotal = curr->product.harga * curr->qty;
        std::cout << std::left
                  << std::setw(4)  << no
                  << std::setw(22) << curr->product.nama
                  << std::setw(5)  << curr->qty
                  << "Rp " << std::setw(12) << std::fixed << std::setprecision(0) << curr->product.harga
                  << "Rp " << subtotal << std::endl;
        curr = curr->next;
        no++;
    }

    std::cout << std::string(55, '-') << std::endl;
    std::cout << "TOTAL: Rp " << std::fixed << std::setprecision(0) << totalHarga() << std::endl;
    std::cout << std::string(55, '-') << std::endl;
}

// ── TOTAL HARGA (traversal semua node) ──────────────────
double CartList::totalHarga() const {
    double total = 0;
    CartNode* curr = head;
    while (curr != nullptr) {
        total += curr->product.harga * curr->qty;
        curr = curr->next;
    }
    return total;
}

// ── GETTER ──────────────────────────────────────────────
bool CartList::isEmpty() const { return head == nullptr; }
int CartList::getSize() const { return size; }
CartNode* CartList::getHead() const { return head; }

bool CartList::produkAda(int productId) const {
    CartNode* curr = head;
    while (curr != nullptr) {
        if (curr->product.id == productId) return true;
        curr = curr->next;
    }
    return false;
}