#include "cart_service.h"
#include <iostream>
#include <iomanip>

using namespace std;

CartService::CartService() {}

// Inisialisasi daftar promo
void CartService::initializePromo() {
    promo.tambahPromo(" Harbolnas! Diskon up to 50%!");
    promo.tambahPromo(" Promo Elektronik - Diskon 30%!");
    promo.tambahPromo(" Fashion Akhir Bulan - Beli 2 Gratis 1!");
    promo.tambahPromo(" Aksesoris Mulai Rp 79.000!");
    promo.tambahPromo(" Gratis Ongkir ke Seluruh Jabodetabek!");
}

// Inisialisasi katalog produk
void CartService::initializeCatalog(Product* products, int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        catalog.addProduct(products[i]);
    }
}

// Setup awal layanan
void CartService::initialize(Product* products, int jumlah) {
    initializePromo();
    initializeCatalog(products, jumlah);
    navStack.push("Katalog");
}

// Menampilkan halaman katalog
void CartService::displayCatalogPage() {
    cout << string(55, '=') << endl;
    cout << "           C-CART - KATALOG PRODUK" << endl;
    cout << string(55, '=') << endl;

    cout << "  ";
    promo.displayCurrent();
    cout << string(55, '-') << endl;

    catalog.displayCurrent();

    cout << endl;
    cout << "  [1] Prev   [2] Next   [3] Tambah ke Keranjang" << endl;
    cout << "  [4] Lihat Semua Produk   [5] Keranjang" << endl;
    cout << "  [6] Next Promo   [0] Keluar" << endl;
}

void CartService::nextProduct() {
    catalog.next();
}

void CartService::prevProduct() {
    catalog.prev();
}

// Menambahkan produk ke keranjang berdasarkan ID
// Mencari produk dari seluruh katalog, bukan hanya produk yang sedang tampil
void CartService::addToCart(int productId) {
    Product* p = catalog.findById(productId);
    if (p != nullptr) {
        cart.addProduct(*p);
        cout << "Produk '" << p->nama << "' berhasil ditambahkan ke keranjang." << endl;
    } else {
        cout << "Produk dengan ID " << productId << " tidak ditemukan di katalog." << endl;
    }
}

void CartService::nextPromo() {
    promo.nextPromo();
    cout << "  Promo: ";
    promo.displayCurrent();
}

// Menampilkan isi keranjang belanja
void CartService::displayCart() {
    cout << string(55, '=') << endl;
    cout << "           C-CART - KERANJANG BELANJA" << endl;
    cout << string(55, '=') << endl;
    cart.display();
    cout << endl;
    cout << "  [1] Hapus Produk   [2] Checkout   [0] Back" << endl;
}

void CartService::removeFromCart(int productId) {
    cart.removeProduct(productId);
}

double CartService::getTotalPrice() const {
    return cart.totalPrice();
}

bool CartService::isCartEmpty() const {
    return cart.isEmpty();
}

// Navigasi ke halaman tertentu
void CartService::navigateToPage(string pageName) {
    navStack.push(pageName);
    cout << "  → Pindah ke: " << pageName << endl;
}

// Kembali ke halaman sebelumnya
string CartService::back() {
    if (!canGoBack()) {
        cout << "  Sudah di halaman pertama, tidak bisa Back." << endl;
        return currentPage();
    }
    navStack.pop();
    cout << "  ← Kembali ke: " << currentPage() << endl;
    return currentPage();
}

bool CartService::canGoBack() const {
    return navStack.canGoBack();
}

string CartService::currentPage() const {
    return navStack.peek();
}

CartList& CartService::getCart() {
    return cart;
}
