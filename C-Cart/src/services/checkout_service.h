#ifndef CHECKOUT_SERVICE_H
#define CHECKOUT_SERVICE_H

#include "../structures/order_queue.h"
#include <string>

// Layanan untuk menangani proses checkout dan antrean pesanan
class CheckoutService {
private:
    OrderQueue queue;
    int nextOrderId;

public:
    CheckoutService();

    // Membuat pesanan baru dan masuk ke antrean
    void createOrder(std::string namaPemesan, std::string kotaTujuan, double totalHarga);

    // Memproses pesanan berikutnya dalam antrean
    void processNextOrder();

    // Mendapatkan akses ke antrean
    OrderQueue& getQueue();

    // Cek apakah antrean kosong
    bool isQueueEmpty() const;

    // Menampilkan semua pesanan aktif
    void displayOrders() const;
};

#endif // CHECKOUT_SERVICE_H
