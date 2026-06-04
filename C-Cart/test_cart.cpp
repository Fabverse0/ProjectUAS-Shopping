#include <iostream>
#include <string>
#include "src/models/product.h"
#include "src/structures/cart_list.h"
#include "src/structures/catalog_list.h"
#include "src/structures/promo_list.h"
#include "src/structures/navigation_stack.h"

using namespace std;

// DATA PRODUK HARDCODED
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

void garis() { cout << string(55, '=') << endl; }
void jeda()  { cout << "\nTekan Enter untuk lanjut..."; cin.get(); cout << endl; }

// TEST 1 - Circular Linked List
void testPromoList() {
    garis();
    cout << "TEST 1 - CIRCULAR LINKED LIST (Banner Promo)" << endl;
    garis();

    PromoList promo;
    promo.tambahPromo("Harbolnas! Diskon up to 50%!");
    promo.tambahPromo("Promo Elektronik - Diskon 30%!");
    promo.tambahPromo("Fashion Akhir Bulan - Beli 2 Gratis 1!");
    promo.tambahPromo("Aksesoris Mulai Rp 79.000!");
    promo.tambahPromo("Gratis Ongkir ke Seluruh Jabodetabek!");

    cout << "\nSemua promo yang tersimpan:" << endl;
    promo.displayAll();

    cout << "\nRotasi banner (sirkuler - node terakhir balik ke pertama):" << endl;
    for (int i = 1; i <= 7; i++) {
        cout << "  Rotasi " << i << ": ";
        promo.displayCurrent();
        promo.nextPromo();
    }

    cout << "\n[OK] Circular Linked List OK - node terakhir kembali ke node pertama!" << endl;
    jeda();
}

// TEST 2 - Double Linked List
void testCatalogList() {
    garis();
    cout << "TEST 2 - DOUBLE LINKED LIST (Navigasi Katalog)" << endl;
    garis();

    CatalogList catalog;
    for (int i = 0; i < jumlahProduk; i++) {
        catalog.addProduct(daftarProduk[i]);
    }

    cout << "\nSemua produk di katalog:" << endl;
    catalog.displayAll();

    cout << "\nTest navigasi NEXT:" << endl;
    cout << "Posisi awal:" << endl; catalog.displayCurrent();
    cout << "\nTekan NEXT" << endl; catalog.next();
    cout << "\nTekan NEXT" << endl; catalog.next();
    cout << "\nTekan NEXT" << endl; catalog.next();

    cout << "\nTest navigasi PREV:" << endl;
    cout << "Tekan PREV" << endl; catalog.prev();
    cout << "\nTekan PREV" << endl; catalog.prev();

    cout << "\nTest batas - PREV dari produk pertama:" << endl;
    catalog.resetToFirst();
    catalog.prev();

    cout << "\n[OK] Double Linked List OK - Prev/Next berjalan!" << endl;
    jeda();
}

// TEST 3 - Single Linked List
void testCartList() {
    garis();
    cout << "TEST 3 - SINGLE LINKED LIST (Keranjang Belanja)" << endl;
    garis();

    CartList cart;

    cout << "\nTambah 3 produk ke keranjang:" << endl;
    cart.addProduct(daftarProduk[0]);
    cart.addProduct(daftarProduk[5]);
    cart.addProduct(daftarProduk[10]);

    cout << "\nIsi keranjang:" << endl;
    cart.display();

    cout << "\nTest tambah produk yang sama (qty bertambah):" << endl;
    cart.addProduct(daftarProduk[0], 2);

    cout << "\nKeranjang setelah update qty:" << endl;
    cart.display();

    cout << "\nHapus Sepatu Nike dari keranjang:" << endl;
    cart.removeProduct(6);

    cout << "\nKeranjang setelah hapus:" << endl;
    cart.display();

    cout << "\nTest hapus produk yang tidak ada:" << endl;
    cart.removeProduct(999);

    cout << "\n[OK] Single Linked List OK - Insert/Delete berjalan!" << endl;
    jeda();
}

// TEST 4 - Stack
void testNavigationStack() {
    garis();
    cout << "TEST 4 - STACK (Navigasi Halaman)" << endl;
    garis();

    NavigationStack nav;

    cout << "\nPush halaman - user navigasi maju:" << endl;
    nav.push("Katalog");
    cout << "  Push: Katalog" << endl;
    nav.push("Detail Produk");
    cout << "  Push: Detail Produk" << endl;
    nav.push("Keranjang");
    cout << "  Push: Keranjang" << endl;
    nav.push("Checkout");
    cout << "  Push: Checkout" << endl;

    cout << "\nRiwayat navigasi sekarang:" << endl;
    nav.display();

    cout << "\nTekan Back (Pop) berkali-kali:" << endl;
    while (nav.canGoBack()) {
        string kembali = nav.pop();
        cout << "  Back dari: " << kembali
             << " | sekarang di: " << nav.peek() << endl;
    }

    cout << "\nTest Back di halaman pertama:" << endl;
    if (!nav.canGoBack()) {
        cout << "  Sudah di halaman pertama, tombol Back tidak aktif!" << endl;
    }

    cout << "\n[OK] Stack OK - Push/Pop navigasi berjalan!" << endl;
    jeda();
}

// MAIN
int main() {
    cout << string(55, '=') << endl;
    cout << "     C-CART - TEST STRUKTUR DATA ORANG 2" << endl;
    cout << "  Cart, Catalog, Promo, Navigation" << endl;
    cout << string(55, '=') << endl;
    cout << "\nAda 4 test yang akan dijalankan." << endl;
    jeda();

    testPromoList();
    testCatalogList();
    testCartList();
    testNavigationStack();

    garis();
    cout << "SEMUA TEST SELESAI!" << endl;
    cout << "[OK] Circular Linked List  - Banner Promo" << endl;
    cout << "[OK] Double Linked List    - Navigasi Katalog" << endl;
    cout << "[OK] Single Linked List    - Keranjang Belanja" << endl;
    cout << "[OK] Stack (Linked List)   - Navigasi Halaman" << endl;
    garis();

    return 0;
}
