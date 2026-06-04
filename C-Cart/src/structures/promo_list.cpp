#include "promo_list.h"
#include <iostream>

using namespace std;

PromoList::PromoList() : head(nullptr), current(nullptr), size(0) {}

PromoList::~PromoList() {
    if (head == nullptr) return;

    PromoNode* curr = head;
    do {
        PromoNode* temp = curr;
        curr = curr->next;
        delete temp;
    } while (curr != head);

    head = current = nullptr;
}

// Menambah promo ke dalam list sirkuler
void PromoList::tambahPromo(string teks) {
    PromoNode* newNode = new PromoNode(teks);

    if (head == nullptr) {
        head = newNode;
        newNode->next = head;
        current = head;
    } else {
        PromoNode* tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }
        tail->next = newNode;
        newNode->next = head;
    }
    size++;
}

// Geser ke promo berikutnya secara sirkuler
void PromoList::nextPromo() {
    if (current == nullptr) return;
    current = current->next;
}

// Menampilkan promo yang sedang aktif
void PromoList::displayCurrent() const {
    if (current == nullptr) {
        cout << "Tidak ada promo." << endl;
        return;
    }
    cout << "* " << current->teks << " *" << endl;
}

// Menampilkan seluruh daftar promo
void PromoList::displayAll() const {
    if (head == nullptr) {
        cout << "Tidak ada promo." << endl;
        return;
    }

    cout << "=== DAFTAR PROMO ===" << endl;
    PromoNode* curr = head;
    int no = 1;
    do {
        cout << no << ". " << curr->teks << endl;
        curr = curr->next;
        no++;
    } while (curr != head);
}

bool PromoList::isEmpty() const { return head == nullptr; }
int PromoList::getSize() const { return size; }
string PromoList::getCurrentTeks() const {
    if (current == nullptr) return "";
    return current->teks;
}
