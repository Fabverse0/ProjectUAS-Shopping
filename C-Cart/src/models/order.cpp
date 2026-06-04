#include "order.h"
#include <iostream>
#include <iomanip>

Order::Order() : orderId(0), namaPemesan(""), kotaTujuan(""), totalHarga(0.0), status("Menunggu") {}

Order::Order(int id, std::string nama, std::string kota, double total)
    : orderId(id), namaPemesan(nama), kotaTujuan(kota), totalHarga(total), status("Menunggu") {}

void Order::updateStatus(std::string newStatus) {
    status = newStatus;
}

void Order::tampilkan() const {
    std::cout << "Order ID: " << orderId << "\n"
              << "Pemesan : " << namaPemesan << "\n"
              << "Tujuan  : " << kotaTujuan << "\n"
              << "Total   : Rp " << std::fixed << std::setprecision(0) << totalHarga << "\n"
              << "Status  : " << status << std::endl;
}
