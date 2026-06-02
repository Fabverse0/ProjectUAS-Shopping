#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

/**
 * @brief Product model - merepresentasikan satu produk di katalog
 */
class Product {
public:
    int id;
    std::string nama;
    std::string kategori;
    double harga;

    Product();
    Product(int id, std::string nama, std::string kategori, double harga);

    void tampilkan() const;
};

#endif // PRODUCT_H