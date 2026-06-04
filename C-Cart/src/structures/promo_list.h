#ifndef PROMO_LIST_H
#define PROMO_LIST_H

#include <string>

// Node untuk Circular Linked List banner promo
struct PromoNode {
    std::string teks;
    PromoNode* next;

    PromoNode(std::string teks) : teks(teks), next(nullptr) {}
};

// Struktur data Circular Linked List untuk banner promo sirkuler
class PromoList {
private:
    PromoNode* head;
    PromoNode* current;
    int size;

public:
    PromoList();
    ~PromoList();

    // Operasi promo
    void tambahPromo(std::string teks);
    void nextPromo();

    // Menampilkan promo
    void displayCurrent() const;
    void displayAll() const;

    // Getter
    bool isEmpty() const;
    int getSize() const;
    std::string getCurrentTeks() const;
};

#endif // PROMO_LIST_H
