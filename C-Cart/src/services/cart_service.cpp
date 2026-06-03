#include "cart_service.h"
#include <iostream>
#include <iomanip>

CartService::CartService() {}

// ── INISIALISASI PROMO (hardcoded 5 node) ────────────────
void CartService::inisialisasiPromo() {
    promo.tambahPromo(" Harbolnas! Diskon up to 50%!");
    promo.tambahPromo(" Promo Elektronik - Diskon 30%!");
    promo.tambahPromo(" Fashion Akhir Bulan - Beli 2 Gratis 1!");
    promo.tambahPromo(" Aksesoris Mulai Rp 79.000!");
    promo.tambahPromo(" Gratis Ongkir ke Seluruh Jabodetabek!");
}

// ── INISIALISASI CATALOG dari data AVL Tree ───────────────
void CartService::inisialisasiCatalog(Product* products, int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        catalog.tambahProduk(products[i]);
    }
}

// ── INISIALISASI UTAMA ────────────────────────────────────
void CartService::inisialisasi(Product* products, int jumlah) {
    inisialisasiPromo();
    inisialisasiCatalog(products, jumlah);
    navStack.push("Katalog");   // halaman awal
}

// ── LAYAR 1: KATALOG ─────────────────────────────────────
void CartService::tampilkanHalamanKatalog() {
    std::cout << std::string(55, '=') << std::endl;
    std::cout << "           C-CART - KATALOG PRODUK" << std::endl;
    std::cout << std::string(55, '=') << std::endl;

    // Tampilkan banner promo (Circular Linked List)
    std::cout << "  ";
    promo.tampilkanCurrent();
    std::cout << std::string(55, '-') << std::endl;

    // Tampilkan produk sekarang (Double Linked List)
    catalog.tampilkanCurrent();

    std::cout << std::endl;
    std::cout << "  [1] Prev   [2] Next   [3] Tambah ke Keranjang" << std::endl;
    std::cout << "  [4] Lihat Semua Produk   [5] Keranjang" << std::endl;
    std::cout << "  [6] Next Promo   [0] Keluar" << std::endl;
}

void CartService::nextProduk() {
    catalog.next();
}

void CartService::prevProduk() {
    catalog.prev();
}

void CartService::tambahKeKeranjang(int productId) {
    // Cari produk di catalog
    CatalogNode* curr = catalog.getCurrent();
    if (curr == nullptr) return;

    // Cari dari head catalog
    // (implementasi sederhana: cari dari current)
    if (curr->product.id == productId) {
        cart.tambahProduk(curr->product);
        return;
    }

    std::cout << "Produk tidak ditemukan." << std::endl;
}

void CartService::nextPromo() {
    promo.nextPromo();
    std::cout << "  Promo: ";
    promo.tampilkanCurrent();
}

// ── LAYAR 2: KERANJANG ───────────────────────────────────
void CartService::tampilkanKeranjang() {
    std::cout << std::string(55, '=') << std::endl;
    std::cout << "           C-CART - KERANJANG BELANJA" << std::endl;
    std::cout << std::string(55, '=') << std::endl;
    cart.tampilkan();
    std::cout << std::endl;
    std::cout << "  [1] Hapus Produk   [2] Checkout   [0] Back" << std::endl;
}

void CartService::hapusDariKeranjang(int productId) {
    cart.hapusProduk(productId);
}

double CartService::getTotalHarga() const {
    return cart.totalHarga();
}

bool CartService::keranjangKosong() const {
    return cart.isEmpty();
}

// ── NAVIGASI ─────────────────────────────────────────────
void CartService::pindahKeHalaman(std::string namaHalaman) {
    navStack.push(namaHalaman);
    std::cout << "  → Pindah ke: " << namaHalaman << std::endl;
}

std::string CartService::back() {
    if (!bisaBack()) {
        std::cout << "  Sudah di halaman pertama, tidak bisa Back." << std::endl;
        return halamanSekarang();
    }
    navStack.pop();
    std::cout << "  ← Kembali ke: " << halamanSekarang() << std::endl;
    return halamanSekarang();
}

bool CartService::bisaBack() const {
    return navStack.bisaBack();
}

std::string CartService::halamanSekarang() const {
    return navStack.peek();
}

// ── GETTER ───────────────────────────────────────────────
CartList& CartService::getCart() {
    return cart;
}