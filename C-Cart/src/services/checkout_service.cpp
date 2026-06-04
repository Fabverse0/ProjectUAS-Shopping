#include "checkout_service.h"
#include <iostream>

CheckoutService::CheckoutService() : nextOrderId(1) {}

void CheckoutService::createOrder(std::string namaPemesan, std::string kotaTujuan, double totalHarga) {
    Order newOrder(nextOrderId++, namaPemesan, kotaTujuan, totalHarga);
    queue.enqueue(newOrder);
    std::cout << "Pesanan berhasil dibuat! Order ID: " << newOrder.orderId << std::endl;
}

void CheckoutService::processNextOrder() {
    if (queue.isEmpty()) {
        std::cout << "Tidak ada pesanan untuk diproses." << std::endl;
        return;
    }

    Order* currentOrder = queue.front();
    
    if (currentOrder->status == "Menunggu") {
        currentOrder->updateStatus("Diproses");
        std::cout << "Pesanan #" << currentOrder->orderId << " sedang diproses." << std::endl;
    } else if (currentOrder->status == "Diproses") {
        currentOrder->updateStatus("Selesai");
        std::cout << "Pesanan #" << currentOrder->orderId << " telah selesai diproses." << std::endl;
        queue.dequeue();
        std::cout << "Pesanan dikeluarkan dari antrean." << std::endl;
    }
}

OrderQueue& CheckoutService::getQueue() {
    return queue;
}

bool CheckoutService::isQueueEmpty() const {
    return queue.isEmpty();
}

void CheckoutService::displayOrders() const {
    queue.displayQueue();
}
