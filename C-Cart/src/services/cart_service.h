#ifndef CART_SERVICE_H
#define CART_SERVICE_H

#include "../structures/cart_list.h"
#include "../structures/catalog_list.h"
#include "../structures/promo_list.h"
#include "../structures/navigation_stack.h"

// CartService - Menghubungkan berbagai struktur data untuk fitur belanja
// Mengelola interaksi antara Keranjang, Katalog, Promo, dan Navigasi
class CartService {
private:
    CartList cart;
    CatalogList catalog;
    PromoList promo;
    NavigationStack navStack;

    void initializePromo();
    void initializeCatalog(Product* products, int jumlah);

public:
    CartService();

    // Setup awal dengan data produk dari AVL Tree
    void initialize(Product* products, int jumlah);

    // Fitur Katalog
    void displayCatalogPage();
    void nextProduct();
    void prevProduct();
    void addToCart(int productId);
    void nextPromo();

    // Fitur Keranjang
    void displayCart();
    void removeFromCart(int productId);
    double getTotalPrice() const;
    bool isCartEmpty() const;

    // Navigasi Halaman
    void navigateToPage(std::string pageName);
    std::string back();
    bool canGoBack() const;
    std::string currentPage() const;

    // Getter untuk akses keranjang
    CartList& getCart();
};

#endif // CART_SERVICE_H
