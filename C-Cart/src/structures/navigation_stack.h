#ifndef NAVIGATION_STACK_H
#define NAVIGATION_STACK_H

#include <string>

// Node untuk Stack navigasi halaman
struct StackNode {
    std::string namaHalaman;
    StackNode* next;

    StackNode(std::string nama) : namaHalaman(nama), next(nullptr) {}
};

// Struktur data Stack (Linked List) untuk menyimpan riwayat halaman
class NavigationStack {
private:
    StackNode* top;
    int size;

public:
    NavigationStack();
    ~NavigationStack();

    // Operasi stack
    void push(std::string namaHalaman);
    std::string pop();
    std::string peek() const;

    // Menampilkan riwayat
    void display() const;

    // Pengecekan
    bool isEmpty() const;
    bool canGoBack() const;
    int getSize() const;
};

#endif // NAVIGATION_STACK_H
