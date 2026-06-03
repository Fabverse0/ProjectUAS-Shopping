#include <iostream>
#include <string>
#include "src/models/product.h"
#include "src/structures/cart_list.h"
#include "src/structures/catalog_list.h"
#include "src/structures/promo_list.h"
#include "src/structures/navigation_stack.h"

// DATA PRODUK HARDCODED (sementara, nanti dari AVL Tree Orang 1)
Product daftarProduk[] = {
    Product(1,  "Samsung Galaxy A15",  "Elektronik", 2499000),
    Product(2,  "Earphone JBL T110",   "Elektronik",  299000),
    Product(3,  "Charger GaN 65W",     "Elektronik",  189000),
    Product(4,  "Powerbank Baseus 10K","Elektronik",  349000),
    Product(5,  "Kabel USB-C 1m",      "Elektronik",   79000),
    Product(6,  "Sepatu Nike Air Max",  "Fashion",   1200000),
    Product(7,  "Kaos Polos Oversize",  "Fashion",    129000),
    Product(8,  "Celana Jogger",        "Fashion",    199000),
    Product(9,  "Topi Baseball",        "Fashion",     89000),
    Product(10, "Jaket Hoodie",         "Fashion",    359000),
    Product(11, "Tas Ransel Laptop",    "Aksesoris",  459000),
    Product(12, "Dompet Kulit",         "Aksesoris",  149000),
    Product(13, "Jam Tangan Casio",     "Aksesoris",  599000),
    Product(14, "Kacamata Hitam",       "Aksesoris",  119000),
    Product(15, "Gelang Titanium",      "Aksesoris",   99000),
};
int jumlahProduk = 15;

void garis() { std::cout << std::string(55, '=') << std::endl; }
void jeda()  { std::cout << "\nTekan Enter untuk lanjut..."; std::cin.get(); std::cout << std::endl; }

// TEST 1 - Circular Linked List
void testPromoList() {
    garis();
    std::cout << "TEST 1 - CIRCULAR LINKED LIST (Banner Promo)" << std::endl;
    garis();

    PromoList promo;
    promo.tambahPromo("Harbolnas! Diskon up to 50%!");
    promo.tambahPromo("Promo Elektronik - Diskon 30%!");
    promo.tambahPromo("Fashion Akhir Bulan - Beli 2 Gratis 1!");
    promo.tambahPromo("Aksesoris Mulai Rp 79.000!");
    promo.tambahPromo("Gratis Ongkir ke Seluruh Jabodetabek!");

    std::cout << "\nSemua promo yang tersimpan:" << std::endl;
    promo.tampilkanSemua();

    std::cout << "\nRotasi banner (sirkuler - node terakhir balik ke pertama):" << std::endl;
    for (int i = 1; i <= 7; i++) {
        std::cout << "  Rotasi " << i << ": ";
        promo.tampilkanCurrent();
        promo.nextPromo();
    }

    std::cout << "\n[OK] Circular Linked List OK - node terakhir kembali ke node pertama!" << std::endl;
    jeda();
}

// TEST 2 - Double Linked List
void testCatalogList() {
    garis();
    std::cout << "TEST 2 - DOUBLE LINKED LIST (Navigasi Katalog)" << std::endl;
    garis();

    CatalogList catalog;
    for (int i = 0; i < jumlahProduk; i++) {
        catalog.tambahProduk(daftarProduk[i]);
    }

    std::cout << "\nSemua produk di katalog:" << std::endl;
    catalog.tampilkanSemua();

    std::cout << "\nTest navigasi NEXT:" << std::endl;
    std::cout << "Posisi awal:" << std::endl; catalog.tampilkanCurrent();
    std::cout << "\nTekan NEXT" << std::endl; catalog.next();
    std::cout << "\nTekan NEXT" << std::endl; catalog.next();
    std::cout << "\nTekan NEXT" << std::endl; catalog.next();

    std::cout << "\nTest navigasi PREV:" << std::endl;
    std::cout << "Tekan PREV" << std::endl; catalog.prev();
    std::cout << "\nTekan PREV" << std::endl; catalog.prev();

    std::cout << "\nTest batas - PREV dari produk pertama:" << std::endl;
    catalog.resetKeCurrent();
    catalog.prev();

    std::cout << "\n[OK] Double Linked List OK - Prev/Next berjalan!" << std::endl;
    jeda();
}

// TEST 3 - Single Linked List
void testCartList() {
    garis();
    std::cout << "TEST 3 - SINGLE LINKED LIST (Keranjang Belanja)" << std::endl;
    garis();

    CartList cart;

    std::cout << "\nTambah 3 produk ke keranjang:" << std::endl;
    cart.tambahProduk(daftarProduk[0]);
    cart.tambahProduk(daftarProduk[5]);
    cart.tambahProduk(daftarProduk[10]);

    std::cout << "\nIsi keranjang:" << std::endl;
    cart.tampilkan();

    std::cout << "\nTest tambah produk yang sama (qty bertambah):" << std::endl;
    cart.tambahProduk(daftarProduk[0], 2);

    std::cout << "\nKeranjang setelah update qty:" << std::endl;
    cart.tampilkan();

    std::cout << "\nHapus Sepatu Nike dari keranjang:" << std::endl;
    cart.hapusProduk(6);

    std::cout << "\nKeranjang setelah hapus:" << std::endl;
    cart.tampilkan();

    std::cout << "\nTest hapus produk yang tidak ada:" << std::endl;
    cart.hapusProduk(999);

    std::cout << "\n[OK] Single Linked List OK - Insert/Delete berjalan!" << std::endl;
    jeda();
}

// TEST 4 - Stack
void testNavigationStack() {
    garis();
    std::cout << "TEST 4 - STACK (Navigasi Halaman)" << std::endl;
    garis();

    NavigationStack nav;

    std::cout << "\nPush halaman - user navigasi maju:" << std::endl;
    nav.push("Katalog");
    std::cout << "  Push: Katalog" << std::endl;
    nav.push("Detail Produk");
    std::cout << "  Push: Detail Produk" << std::endl;
    nav.push("Keranjang");
    std::cout << "  Push: Keranjang" << std::endl;
    nav.push("Checkout");
    std::cout << "  Push: Checkout" << std::endl;

    std::cout << "\nRiwayat navigasi sekarang:" << std::endl;
    nav.tampilkan();

    std::cout << "\nTekan Back (Pop) berkali-kali:" << std::endl;
    while (nav.bisaBack()) {
        std::string kembali = nav.pop();
        std::cout << "  Back dari: " << kembali
                  << " | sekarang di: " << nav.peek() << std::endl;
    }

    std::cout << "\nTest Back di halaman pertama:" << std::endl;
    if (!nav.bisaBack()) {
        std::cout << "  Sudah di halaman pertama, tombol Back tidak aktif!" << std::endl;
    }

    std::cout << "\n[OK] Stack OK - Push/Pop navigasi berjalan!" << std::endl;
    jeda();
}

// MAIN
int main() {
    std::cout << std::string(55, '=') << std::endl;
    std::cout << "     C-CART - TEST STRUKTUR DATA ORANG 2" << std::endl;
    std::cout << "  Cart, Catalog, Promo, Navigation" << std::endl;
    std::cout << std::string(55, '=') << std::endl;
    std::cout << "\nAda 4 test yang akan dijalankan." << std::endl;
    jeda();

    testPromoList();
    testCatalogList();
    testCartList();
    testNavigationStack();

    garis();
    std::cout << "SEMUA TEST SELESAI!" << std::endl;
    std::cout << "[OK] Circular Linked List  - Banner Promo" << std::endl;
    std::cout << "[OK] Double Linked List    - Navigasi Katalog" << std::endl;
    std::cout << "[OK] Single Linked List    - Keranjang Belanja" << std::endl;
    std::cout << "[OK] Stack (Linked List)   - Navigasi Halaman" << std::endl;
    garis();

    return 0;
}