#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

// Model untuk data produk
class Product {
public:
    int id;
    std::string nama;
    std::string kategori;
    double harga;

    Product();
    Product(int id, std::string nama, std::string kategori, double harga);

    // Menampilkan info produk singkat
    void display() const;
};

#endif // PRODUCT_H
