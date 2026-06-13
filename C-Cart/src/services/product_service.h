#ifndef PRODUCT_SERVICE_H
#define PRODUCT_SERVICE_H

#include "../models/product.h"
#include "../structures/avltree.h"
#include "../structures/bst.h"
#include <vector>

// Layanan untuk mengelola operasi produk (Katalog Utama)
class ProductService {
private:
    AVLTree catalog;
    
public:
    ProductService();

    // Memuat 15 produk hardcoded ke katalog
    void loadHardcodedProducts();
    
    // Mendapatkan semua produk terurut berdasarkan ID
    std::vector<Product> getAllProducts();

    // Mencari produk berdasarkan ID menggunakan AVL search
    Product* findProductById(int id);

    // Menambah produk baru
    void insertProduct(Product p);

    // Menghapus produk berdasarkan ID
    void deleteProduct(int id);

    // Mendapatkan produk terurut berdasarkan harga menggunakan BST
    std::vector<Product> getProductsSortedByPrice();
    
    // Implementasi Binary Search untuk pencarian ID
    Product* binarySearchById(const std::vector<Product>& products, int id);

    // Fungsi demo untuk menyeimbangkan AVL Tree
    void insertDemoProduct();
    void deleteDemoProduct();

    // Fungsi pembantu untuk traversal
    std::vector<Product> getInorder();
    std::vector<Product> getPreorder();
    std::vector<Product> getPostorder();
};

#endif // PRODUCT_SERVICE_H
