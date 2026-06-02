#ifndef PROMO_LIST_H
#define PROMO_LIST_H

#include <string>

/**
 * @brief Node untuk Circular Linked List banner promo
 */
struct PromoNode {
    std::string teks;
    PromoNode* next;

    PromoNode(std::string teks) : teks(teks), next(nullptr) {}
};

/**
 * @brief Circular Linked List untuk banner promo
 * Node terakhir menunjuk kembali ke node pertama (sirkuler)
 * Digunakan untuk rotasi banner promo di halaman utama
 */
class PromoList {
private:
    PromoNode* head;
    PromoNode* current;
    int size;

public:
    PromoList();
    ~PromoList();

    void tambahPromo(std::string teks);   // insert node promo
    void nextPromo();                      // geser ke promo berikutnya
    void tampilkanCurrent() const;         // tampilkan promo sekarang
    void tampilkanSemua() const;           // tampilkan semua promo

    bool isEmpty() const;
    int getSize() const;
    std::string getCurrentTeks() const;
};

#endif // PROMO_LIST_H