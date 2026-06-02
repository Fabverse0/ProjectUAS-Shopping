#include "promo_list.h"
#include <iostream>

PromoList::PromoList() : head(nullptr), current(nullptr), size(0) {}

PromoList::~PromoList() {
    if (head == nullptr) return;

    // Putus sirkuler dulu sebelum hapus
    PromoNode* curr = head;
    do {
        PromoNode* temp = curr;
        curr = curr->next;
        delete temp;
    } while (curr != head);

    head = current = nullptr;
}

// ── TAMBAH PROMO ─────────────────────────────────────────
void PromoList::tambahPromo(std::string teks) {
    PromoNode* newNode = new PromoNode(teks);

    if (head == nullptr) {
        head = newNode;
        newNode->next = head;   // sirkuler ke diri sendiri
        current = head;
    } else {
        // Cari node terakhir
        PromoNode* tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }
        tail->next = newNode;
        newNode->next = head;   // sirkuler kembali ke head
    }
    size++;
}

// ── NEXT PROMO (sirkuler) ────────────────────────────────
void PromoList::nextPromo() {
    if (current == nullptr) return;
    current = current->next;   // otomatis kembali ke head jika di node terakhir
}

// ── TAMPILKAN PROMO SEKARANG ─────────────────────────────
void PromoList::tampilkanCurrent() const {
    if (current == nullptr) {
        std::cout << "Tidak ada promo." << std::endl;
        return;
    }
    std::cout << "* " << current->teks << " *" << std::endl;
}

// ── TAMPILKAN SEMUA PROMO ────────────────────────────────
void PromoList::tampilkanSemua() const {
    if (head == nullptr) {
        std::cout << "Tidak ada promo." << std::endl;
        return;
    }

    std::cout << "=== DAFTAR PROMO ===" << std::endl;
    PromoNode* curr = head;
    int no = 1;
    do {
        std::cout << no << ". " << curr->teks << std::endl;
        curr = curr->next;
        no++;
    } while (curr != head);
}

// ── GETTER ───────────────────────────────────────────────
bool PromoList::isEmpty() const { return head == nullptr; }
int PromoList::getSize() const { return size; }
std::string PromoList::getCurrentTeks() const {
    if (current == nullptr) return "";
    return current->teks;
}