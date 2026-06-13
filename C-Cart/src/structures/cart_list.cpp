#include "cart_list.h"
#include <iostream>
#include <iomanip>

using namespace std;

CartList::CartList() : head(nullptr), size(0) {}

CartList::~CartList() {
    clear();
}

// Menambah produk ke keranjang
void CartList::addProduct(Product p, int qty) {
    CartNode* curr = head;
    while (curr != nullptr) {
        if (curr->product.id == p.id) {
            curr->qty += qty;
            cout << "> Qty " << p.nama << " diupdate menjadi " << curr->qty << endl;
            return;
        }
        curr = curr->next;
    }

    CartNode* newNode = new CartNode(p, qty);
    newNode->next = head;
    head = newNode;
    size++;
    cout << "> " << p.nama << " ditambahkan ke keranjang!" << endl;
}

// Menghapus produk dari keranjang
void CartList::removeProduct(int productId) {
    if (head == nullptr) {
        cout << "Keranjang sudah kosong!" << endl;
        return;
    }

    if (head->product.id == productId) {
        CartNode* temp = head;
        head = head->next;
        cout << "> " << temp->product.nama << " dihapus dari keranjang." << endl;
        delete temp;
        size--;
        return;
    }

    CartNode* curr = head;
    while (curr->next != nullptr) {
        if (curr->next->product.id == productId) {
            CartNode* temp = curr->next;
            curr->next = temp->next;
            cout << "> " << temp->product.nama << " dihapus dari keranjang." << endl;
            delete temp;
            size--;
            return;
        }
        curr = curr->next;
    }

    cout << "Produk tidak ditemukan di keranjang." << endl;
}

// Mengupdate jumlah produk
void CartList::updateQty(int productId, int qty) {
    CartNode* curr = head;
    while (curr != nullptr) {
        if (curr->product.id == productId) {
            if (qty <= 0) {
                removeProduct(productId);
            } else {
                curr->qty = qty;
                cout << "> Qty diupdate menjadi " << qty << endl;
            }
            return;
        }
        curr = curr->next;
    }
    cout << "Produk tidak ditemukan di keranjang." << endl;
}

// Mengosongkan keranjang
void CartList::clear() {
    CartNode* curr = head;
    while (curr != nullptr) {
        CartNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = nullptr;
    size = 0;
}

// Menampilkan daftar belanjaan
void CartList::display() const {
    if (head == nullptr) {
        cout << "  Keranjang masih kosong." << endl;
        return;
    }

    cout << string(55, '-') << endl;
    cout << left
         << setw(4)  << "No"
         << setw(22) << "Nama Produk"
         << setw(5)  << "Qty"
         << setw(15) << "Harga Satuan"
         << "Subtotal" << endl;
    cout << string(55, '-') << endl;

    CartNode* curr = head;
    int no = 1;
    while (curr != nullptr) {
        double subtotal = curr->product.harga * curr->qty;
        cout << left
             << setw(4)  << no
             << setw(22) << curr->product.nama
             << setw(5)  << curr->qty
             << "Rp " << setw(12) << fixed << setprecision(0) << curr->product.harga
             << "Rp " << subtotal << endl;
        curr = curr->next;
        no++;
    }

    cout << string(55, '-') << endl;
    cout << "TOTAL: Rp " << fixed << setprecision(0) << totalPrice() << endl;
    cout << string(55, '-') << endl;
}

// Menghitung total harga belanjaan
double CartList::totalPrice() const {
    double total = 0;
    CartNode* curr = head;
    while (curr != nullptr) {
        total += curr->product.harga * curr->qty;
        curr = curr->next;
    }
    return total;
}

bool CartList::isEmpty() const { return head == nullptr; }
int CartList::getSize() const { return size; }
CartNode* CartList::getHead() const { return head; }

bool CartList::hasProduct(int productId) const {
    CartNode* curr = head;
    while (curr != nullptr) {
        if (curr->product.id == productId) return true;
        curr = curr->next;
    }
    return false;
}
