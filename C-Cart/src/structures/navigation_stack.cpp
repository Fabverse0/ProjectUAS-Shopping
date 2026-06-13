#include "navigation_stack.h"
#include <iostream>

using namespace std;

NavigationStack::NavigationStack() : top(nullptr), size(0) {}

NavigationStack::~NavigationStack() {
    while (!isEmpty()) {
        pop();
    }
}

// Menambahkan halaman baru ke riwayat
void NavigationStack::push(string namaHalaman) {
    StackNode* newNode = new StackNode(namaHalaman);
    newNode->next = top;
    top = newNode;
    size++;
}

// Menghapus dan mendapatkan halaman terakhir
string NavigationStack::pop() {
    if (isEmpty()) {
        return "";
    }
    StackNode* temp = top;
    string nama = temp->namaHalaman;
    top = top->next;
    delete temp;
    size--;
    return nama;
}

// Melihat halaman aktif sekarang
string NavigationStack::peek() const {
    if (isEmpty()) return "";
    return top->namaHalaman;
}

// Menampilkan riwayat perpindahan halaman
void NavigationStack::display() const {
    if (isEmpty()) {
        cout << "  Stack navigasi kosong." << endl;
        return;
    }

    cout << "  Riwayat navigasi (atas = sekarang):" << endl;
    StackNode* curr = top;
    int no = 1;
    while (curr != nullptr) {
        cout << "  " << no << ". " << curr->namaHalaman;
        if (no == 1) cout << " < (sekarang)";
        cout << endl;
        curr = curr->next;
        no++;
    }
}

bool NavigationStack::isEmpty() const { return top == nullptr; }
bool NavigationStack::canGoBack() const { return size > 1; }
int NavigationStack::getSize() const { return size; }
