#include "order_queue.h"
#include <iostream>

OrderQueue::OrderQueue() : frontNode(nullptr), rearNode(nullptr), size(0) {}

OrderQueue::~OrderQueue() {
    clear();
}

void OrderQueue::enqueue(Order order) {
    OrderNode* newNode = new OrderNode(order);
    if (isEmpty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    size++;
}

void OrderQueue::dequeue() {
    if (isEmpty()) return;

    OrderNode* temp = frontNode;
    frontNode = frontNode->next;
    
    if (frontNode == nullptr) {
        rearNode = nullptr;
    }

    delete temp;
    size--;
}

Order* OrderQueue::front() {
    if (isEmpty()) return nullptr;
    return &(frontNode->data);
}

bool OrderQueue::isEmpty() const {
    return frontNode == nullptr;
}

int OrderQueue::getSize() const {
    return size;
}

void OrderQueue::displayQueue() const {
    if (isEmpty()) {
        std::cout << "Antrean pesanan kosong." << std::endl;
        return;
    }

    OrderNode* current = frontNode;
    std::cout << "--- Daftar Antrean Pesanan ---" << std::endl;
    while (current != nullptr) {
        current->data.tampilkan();
        std::cout << "-----------------------------" << std::endl;
        current = current->next;
    }
}

void OrderQueue::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}
