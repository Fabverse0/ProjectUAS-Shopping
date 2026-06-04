#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "../models/order.h"

// Node untuk linked list antrean pesanan
struct OrderNode {
    Order data;
    OrderNode* next;
    OrderNode(Order o) : data(o), next(nullptr) {}
};

// Struktur data Queue (FIFO) untuk mengelola antrean pesanan
class OrderQueue {
private:
    OrderNode* frontNode;
    OrderNode* rearNode;
    int size;

public:
    OrderQueue();
    ~OrderQueue();

    // Operasi queue
    void enqueue(Order order);
    void dequeue();
    Order* front();

    // Pengecekan
    bool isEmpty() const;
    int getSize() const;

    // Menampilkan semua antrean
    void display() const;

    // Membersihkan antrean
    void clear();
};

#endif // ORDER_QUEUE_H
