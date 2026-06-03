#include "product.h"
#include <iostream>
#include <iomanip>

Product::Product() : id(0), nama(""), kategori(""), harga(0.0) {}

Product::Product(int id, std::string nama, std::string kategori, double harga)
    : id(id), nama(nama), kategori(kategori), harga(harga) {}

void Product::tampilkan() const {
    std::cout << "[" << id << "] " << nama
              << " | " << kategori
              << " | Rp " << std::fixed << std::setprecision(0) << harga
              << std::endl;
}