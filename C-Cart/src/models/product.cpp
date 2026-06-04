#include "product.h"
#include <iostream>
#include <iomanip>

using namespace std;

Product::Product() : id(0), nama(""), kategori(""), harga(0.0) {}

Product::Product(int id, string nama, string kategori, double harga)
    : id(id), nama(nama), kategori(kategori), harga(harga) {}

void Product::display() const {
    cout << "[" << id << "] " << nama
         << " | " << kategori
         << " | Rp " << fixed << setprecision(0) << harga
         << endl;
}
