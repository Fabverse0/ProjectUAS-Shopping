#include "catalog_list.h"
#include <iostream>
#include <iomanip>

using namespace std;

CatalogList::CatalogList() : head(nullptr), tail(nullptr), current(nullptr), size(0) {}

CatalogList::~CatalogList() {
    CatalogNode* curr = head;
    while (curr != nullptr) {
        CatalogNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

// Menambah produk ke akhir list
void CatalogList::addProduct(Product p) {
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

// Pindah ke produk selanjutnya
void CatalogList::next() {
    if (current == nullptr) {
        cout << "Katalog kosong." << endl;
        return;
    }
    if (current->next == nullptr) {
        cout << "Sudah di produk terakhir." << endl;
        return;
    }
    current = current->next;
    displayCurrent();
}

// Pindah ke produk sebelumnya
void CatalogList::prev() {
    if (current == nullptr) {
        cout << "Katalog kosong." << endl;
        return;
    }
    if (current->prev == nullptr) {
        cout << "Sudah di produk pertama." << endl;
        return;
    }
    current = current->prev;
    displayCurrent();
}

// Kembali ke produk pertama
void CatalogList::resetToFirst() {
    current = head;
}

// Mencari produk berdasarkan ID
Product* CatalogList::findById(int productId) {
    CatalogNode* curr = head;
    while (curr != nullptr) {
        if (curr->product.id == productId) {
            return &(curr->product);
        }
        curr = curr->next;
    }
    return nullptr;
}

// Tampilkan data produk yang sedang dipilih
void CatalogList::displayCurrent() const {
    if (current == nullptr) {
        cout << "Katalog kosong." << endl;
        return;
    }

    cout << string(45, '=') << endl;
    cout << "  ID       : " << current->product.id << endl;
    cout << "  Nama     : " << current->product.nama << endl;
    cout << "  Kategori : " << current->product.kategori << endl;
    cout << "  Harga    : Rp " << fixed << setprecision(0)
         << current->product.harga << endl;
    cout << string(45, '=') << endl;

    cout << "  ";
    if (current->prev) cout << "[< PREV] ";
    else cout << "         ";
    cout << "           ";
    if (current->next) cout << "[NEXT >]";
    cout << endl;
}

// Tampilkan semua produk dalam katalog
void CatalogList::displayAll() const {
    if (head == nullptr) {
        cout << "Katalog kosong." << endl;
        return;
    }

    cout << string(55, '-') << endl;
    cout << left
         << setw(4)  << "ID"
         << setw(25) << "Nama Produk"
         << setw(15) << "Kategori"
         << "Harga" << endl;
    cout << string(55, '-') << endl;

    CatalogNode* curr = head;
    while (curr != nullptr) {
        cout << left
             << setw(4)  << curr->product.id
             << setw(25) << curr->product.nama
             << setw(15) << curr->product.kategori
             << "Rp " << fixed << setprecision(0)
             << curr->product.harga << endl;
        curr = curr->next;
    }
    cout << string(55, '-') << endl;
}

bool CatalogList::isEmpty() const { return head == nullptr; }
int CatalogList::getSize() const { return size; }
CatalogNode* CatalogList::getCurrent() const { return current; }
Product CatalogList::getCurrentProduct() const { return current->product; }
