#ifndef CART_SERVICE_H
#define CART_SERVICE_H

#include "../structures/cart_list.h"
#include "../structures/catalog_list.h"
#include "../structures/promo_list.h"
#include "../structures/navigation_stack.h"

/**
 * @brief CartService - menghubungkan semua struktur data Orang 2
 * Mengelola interaksi antara Cart, Catalog, Promo, dan Navigation
 */
class CartService {
private:
    CartList cart;
    CatalogList catalog;
    PromoList promo;
    NavigationStack navStack;

    void inisialisasiPromo();
    void inisialisasiCatalog(Product* products, int jumlah);

public:
    CartService();

    // Setup awal — dipanggil setelah data produk dari AVL Tree tersedia
    void inisialisasi(Product* products, int jumlah);

    // ── LAYAR 1: Katalog ──
    void tampilkanHalamanKatalog();
    void nextProduk();
    void prevProduk();
    void tambahKeKeranjang(int productId);
    void nextPromo();

    // ── LAYAR 2: Keranjang ──
    void tampilkanKeranjang();
    void hapusDariKeranjang(int productId);
    double getTotalHarga() const;
    bool keranjangKosong() const;

    // ── NAVIGASI ──
    void pindahKeHalaman(std::string namaHalaman);
    std::string back();
    bool bisaBack() const;
    std::string halamanSekarang() const;

    // Getter untuk dipakai Orang 3 (Checkout)
    CartList& getCart();
};

#endif // CART_SERVICE_H