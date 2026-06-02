#include "catalog_list.h"
#include <iostream>
#include <iomanip>

CatalogList::CatalogList() : head(nullptr), tail(nullptr), current(nullptr), size(0) {}

CatalogList::~CatalogList() {
    CatalogNode* curr = head;
    while (curr != nullptr) {
        CatalogNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

// ── TAMBAH PRODUK (insert di belakang) ──────────────────
void CatalogList::tambahProduk(Product p) {
    CatalogNode* newNode = new CatalogNode(p);

    if (head == nullptr) {
        head = tail = current = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

// ── NEXT (geser ke kanan) ────────────────────────────────
void CatalogList::next() {
    if (current == nullptr) {
        std::cout << "Katalog kosong." << std::endl;
        return;
    }
    if (current->next == nullptr) {
        std::cout << "Sudah di produk terakhir." << std::endl;
        return;
    }
    current = current->next;
    tampilkanCurrent();
}

// ── PREV (geser ke kiri) ─────────────────────────────────
void CatalogList::prev() {
    if (current == nullptr) {
        std::cout << "Katalog kosong." << std::endl;
        return;
    }
    if (current->prev == nullptr) {
        std::cout << "Sudah di produk pertama." << std::endl;
        return;
    }
    current = current->prev;
    tampilkanCurrent();
}

// ── RESET KE AWAL ───────────────────────────────────────
void CatalogList::resetKeCurrent() {
    current = head;
}

// ── TAMPILKAN PRODUK SEKARANG ────────────────────────────
void CatalogList::tampilkanCurrent() const {
    if (current == nullptr) {
        std::cout << "Katalog kosong." << std::endl;
        return;
    }

    std::cout << std::string(45, '=') << std::endl;
    std::cout << "  ID       : " << current->product.id << std::endl;
    std::cout << "  Nama     : " << current->product.nama << std::endl;
    std::cout << "  Kategori : " << current->product.kategori << std::endl;
    std::cout << "  Harga    : Rp " << std::fixed << std::setprecision(0)
              << current->product.harga << std::endl;
    std::cout << std::string(45, '=') << std::endl;

    // Tampilkan info navigasi
    std::cout << "  ";
    if (current->prev) std::cout << "[< PREV] ";
    else std::cout << "         ";
    std::cout << "           ";
    if (current->next) std::cout << "[NEXT >]";
    std::cout << std::endl;
}

// ── TAMPILKAN SEMUA ──────────────────────────────────────
void CatalogList::tampilkanSemua() const {
    if (head == nullptr) {
        std::cout << "Katalog kosong." << std::endl;
        return;
    }

    std::cout << std::string(55, '-') << std::endl;
    std::cout << std::left
              << std::setw(4)  << "ID"
              << std::setw(25) << "Nama Produk"
              << std::setw(15) << "Kategori"
              << "Harga" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    CatalogNode* curr = head;
    while (curr != nullptr) {
        std::cout << std::left
                  << std::setw(4)  << curr->product.id
                  << std::setw(25) << curr->product.nama
                  << std::setw(15) << curr->product.kategori
                  << "Rp " << std::fixed << std::setprecision(0)
                  << curr->product.harga << std::endl;
        curr = curr->next;
    }
    std::cout << std::string(55, '-') << std::endl;
}

// ── GETTER ──────────────────────────────────────────────
bool CatalogList::isEmpty() const { return head == nullptr; }
int CatalogList::getSize() const { return size; }
CatalogNode* CatalogList::getCurrent() const { return current; }
Product CatalogList::getCurrentProduct() const { return current->product; }