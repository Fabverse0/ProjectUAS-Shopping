#include "order.h"
#include <iostream>
#include <iomanip>

using namespace std;

Order::Order() : orderId(0), namaPemesan(""), kotaTujuan(""), totalHarga(0.0), status("Menunggu") {}

Order::Order(int id, string nama, string kota, double total)
    : orderId(id), namaPemesan(nama), kotaTujuan(kota), totalHarga(total), status("Menunggu") {}

void Order::updateStatus(string newStatus) {
    status = newStatus;
}

void Order::display() const {
    cout << "Order ID: " << orderId << "\n"
         << "Pemesan : " << namaPemesan << "\n"
         << "Tujuan  : " << kotaTujuan << "\n"
         << "Total   : Rp " << fixed << setprecision(0) << totalHarga << "\n"
         << "Status  : " << status << endl;
}
