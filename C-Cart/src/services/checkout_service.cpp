#include "checkout_service.h"
#include <iostream>

using namespace std;

CheckoutService::CheckoutService() : nextOrderId(1) {}

void CheckoutService::createOrder(string namaPemesan, string kotaTujuan, double totalHarga) {
    Order newOrder(nextOrderId++, namaPemesan, kotaTujuan, totalHarga);
    queue.enqueue(newOrder);
    cout << "Pesanan berhasil dibuat! Order ID: " << newOrder.orderId << endl;
}

void CheckoutService::processNextOrder() {
    if (queue.isEmpty()) {
        cout << "Tidak ada pesanan untuk diproses." << endl;
        return;
    }

    Order* currentOrder = queue.front();
    
    if (currentOrder->status == "Menunggu") {
        currentOrder->updateStatus("Diproses");
        cout << "Pesanan #" << currentOrder->orderId << " sedang diproses." << endl;
    } else if (currentOrder->status == "Diproses") {
        currentOrder->updateStatus("Selesai");
        cout << "Pesanan #" << currentOrder->orderId << " telah selesai diproses." << endl;
        queue.dequeue();
        cout << "Pesanan dikeluarkan dari antrean." << endl;
    }
}

OrderQueue& CheckoutService::getQueue() {
    return queue;
}

bool CheckoutService::isQueueEmpty() const {
    return queue.isEmpty();
}

void CheckoutService::displayOrders() const {
    queue.display();
}
