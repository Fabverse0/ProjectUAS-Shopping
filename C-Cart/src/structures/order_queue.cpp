#include "order_queue.h"
#include <iostream>

using namespace std;

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

void OrderQueue::display() const {
    if (isEmpty()) {
        cout << "Antrean pesanan kosong." << endl;
        return;
    }

    OrderNode* current = frontNode;
    cout << "--- Daftar Antrean Pesanan ---" << endl;
    while (current != nullptr) {
        current->data.display();
        cout << "-----------------------------" << endl;
        current = current->next;
    }
}

void OrderQueue::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}
