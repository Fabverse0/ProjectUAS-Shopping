#ifndef ORDER_H
#define ORDER_H

#include <string>

// Model untuk data pesanan (setelah checkout)
class Order {
public:
    int orderId;
    std::string namaPemesan;
    std::string kotaTujuan;
    double totalHarga;
    std::string status;

    Order();
    Order(int id, std::string nama, std::string kota, double total);

    // Mengupdate status pesanan
    void updateStatus(std::string newStatus);

    // Menampilkan detail pesanan
    void display() const;
};

#endif // ORDER_H
