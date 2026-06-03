#include "navigation_stack.h"
#include <iostream>

NavigationStack::NavigationStack() : top(nullptr), size(0) {}

NavigationStack::~NavigationStack() {
    while (!isEmpty()) {
        pop();
    }
}

// ── PUSH (masuk ke halaman baru) ─────────────────────────
void NavigationStack::push(std::string namaHalaman) {
    StackNode* newNode = new StackNode(namaHalaman);
    newNode->next = top;
    top = newNode;
    size++;
}

// ── POP (kembali ke halaman sebelumnya) ──────────────────
std::string NavigationStack::pop() {
    if (isEmpty()) {
        return "";
    }
    StackNode* temp = top;
    std::string nama = temp->namaHalaman;
    top = top->next;
    delete temp;
    size--;
    return nama;
}

// ── PEEK (lihat halaman sekarang) ────────────────────────
std::string NavigationStack::peek() const {
    if (isEmpty()) return "";
    return top->namaHalaman;
}

// ── TAMPILKAN RIWAYAT NAVIGASI ───────────────────────────
void NavigationStack::tampilkan() const {
    if (isEmpty()) {
        std::cout << "  Stack navigasi kosong." << std::endl;
        return;
    }

    std::cout << "  Riwayat navigasi (atas = sekarang):" << std::endl;
    StackNode* curr = top;
    int no = 1;
    while (curr != nullptr) {
        std::cout << "  " << no << ". " << curr->namaHalaman;
        if (no == 1) std::cout << " < (sekarang)";
        std::cout << std::endl;
        curr = curr->next;
        no++;
    }
}

// ── GETTER ───────────────────────────────────────────────
bool NavigationStack::isEmpty() const { return top == nullptr; }
bool NavigationStack::bisaBack() const { return size > 1; }
int NavigationStack::getSize() const { return size; }