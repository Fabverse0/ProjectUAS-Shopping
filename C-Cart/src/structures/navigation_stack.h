#ifndef NAVIGATION_STACK_H
#define NAVIGATION_STACK_H

#include <string>

/**
 * @brief Node untuk Stack navigasi halaman
 */
struct StackNode {
    std::string namaHalaman;
    StackNode* next;

    StackNode(std::string nama) : namaHalaman(nama), next(nullptr) {}
};

/**
 * @brief Stack (Linked List) untuk riwayat navigasi halaman
 * Push saat pindah halaman maju
 * Pop saat tombol Back ditekan
 * Tombol Back tidak aktif saat Stack kosong (di Layar 1)
 */
class NavigationStack {
private:
    StackNode* top;
    int size;

public:
    NavigationStack();
    ~NavigationStack();

    void push(std::string namaHalaman);   // masuk ke halaman baru
    std::string pop();                     // kembali ke halaman sebelumnya
    std::string peek() const;             // lihat halaman sekarang
    void tampilkan() const;               // tampilkan riwayat navigasi

    bool isEmpty() const;
    bool bisaBack() const;                // cek apakah bisa Back
    int getSize() const;
};

#endif // NAVIGATION_STACK_H