// Guard preprocessor untuk mencegah include ganda pada file header
#ifndef CART_LIST_H
// Definisikan simbol CART_LIST_H sebagai tanda file sudah di-include
#define CART_LIST_H

// Include library string untuk tipe data std::string (menyimpan nama, asal barang)
#include <string>
// Include library vector untuk tipe data std::vector (menyimpan kumpulan data dinamis)
#include <vector>

// Definisikan struct CartNode untuk mewakili satu item dalam keranjang belanja (Single Linked List)
struct CartNode {
    // Menyimpan ID produk yang ada di keranjang
    int id;
    // Menyimpan nama produk yang ada di keranjang
    std::string name;
    // Menyimpan harga produk saat ditambahkan ke keranjang
    long price;
    // Menyimpan lokasi asal barang (hub/gudang) untuk fitur split-order
    std::string origin; // Menyimpan lokasi asal barang yang dimasukkan keranjang
    // Pointer ke node berikutnya dalam linked list keranjang
    CartNode* next;
};

// Definisikan class CartList untuk merepresentasikan keranjang belanja pelanggan
class CartList {
private:
    // Pointer ke node pertama (awal) dalam keranjang belanja
    CartNode* head;

public:
    // Konstruktor: inisialisasi keranjang kosong dengan head = nullptr
    CartList() : head(nullptr) {}

    // Fungsi untuk mengecek apakah keranjang kosong (tidak ada item)
    bool isEmpty() { return head == nullptr; }

    // Fungsi untuk menambahkan satu item produk ke keranjang belanja
    void insertItem(int id, std::string name, long price, std::string origin) {
        // Buat node baru untuk item produk dengan data yang diberikan
        CartNode* newNode = new CartNode{id, name, price, origin, nullptr};
        // Jika keranjang kosong, item baru menjadi head (item pertama)
        if (!head) {
            head = newNode;
        } 
        // Jika keranjang tidak kosong
        else {
            // Traverse ke akhir linked list untuk menemukan posisi insert
            CartNode* temp = head;
            while (temp->next) temp = temp->next;
            // Sambungkan node baru ke akhir linked list
            temp->next = newNode;
        }
    }

    // Fungsi untuk menghapus satu item dari keranjang berdasarkan index-nya
    void deleteNodeAt(int index) {
        // Jika keranjang kosong, tidak ada yang bisa dihapus
        if (!head) return;
        // Jika index adalah 0 (item pertama dalam keranjang)
        if (index == 0) {
            // Simpan pointer head saat ini dalam temp
            CartNode* temp = head;
            // Update head untuk menunjuk ke node berikutnya
            head = head->next;
            // Hapus node lama dari memori
            delete temp;
            return;
        }
        // Traverse linked list untuk menemukan node sebelum target (index - 1)
        CartNode* temp = head;
        for (int i = 0; temp != nullptr && i < index - 1; i++) {
            temp = temp->next;
        }
        // Jika node tidak ditemukan atau node berikutnya tidak ada
        if (!temp || !temp->next) return;
        // Simpan pointer ke node yang akan dihapus dan node setelahnya
        CartNode* nextNode = temp->next->next;
        // Hapus node target dari memori
        delete temp->next;
        // Sambungkan kembali linked list tanpa node yang dihapus
        temp->next = nextNode;
    }

    // Fungsi untuk mengosongkan seluruh keranjang belanja (hapus semua item)
    void clearList() {
        // Selama masih ada node di keranjang
        while (head) {
            // Simpan pointer head saat ini dalam temp
            CartNode* temp = head;
            // Update head untuk menunjuk ke node berikutnya
            head = head->next;
            // Hapus node lama dari memori
            delete temp;
        }
    }

    // Fungsi untuk menghitung total harga semua item dalam keranjang
    long calculateTotal() {
        // Inisialisasi total dengan 0 (nilai awal)
        long total = 0;
        // Traverse seluruh linked list dari head hingga akhir
        CartNode* temp = head;
        while (temp) {
            // Tambahkan harga item saat ini ke total
            total += temp->price;
            // Geser pointer ke node berikutnya
            temp = temp->next;
        }
        // Kembalikan total harga semua item
        return total;
    }

    // Fungsi untuk mendapatkan daftar string informasi semua item dalam keranjang
    std::vector<std::string> getAllItemsString() {
        // Buat vector kosong untuk menyimpan string item
        std::vector<std::string> res;
        // Traverse seluruh linked list dari head hingga akhir
        CartNode* temp = head;
        while (temp) {
            // Format string: nama produk + harga + asal gudang
            res.push_back(temp->name + " (Rp " + std::to_string(temp->price) + ") [Asal: " + temp->origin + "]");
            // Geser pointer ke node berikutnya
            temp = temp->next;
        }
        // Kembalikan vector berisi string informasi semua item
        return res;
    }

    // HELPER METHODS UNTUK KEBUTUHAN SPLIT-ORDER DI MAINWINDOW
    // Fungsi untuk mendapatkan daftar lokasi asal (origin) dari semua item
    std::vector<std::string> getAllOrigins() {
        // Buat vector kosong untuk menyimpan lokasi asal
        std::vector<std::string> res;
        // Traverse seluruh linked list dari head hingga akhir
        CartNode* temp = head;
        while (temp) {
            // Tambahkan lokasi asal item ke vector
            res.push_back(temp->origin);
            // Geser pointer ke node berikutnya
            temp = temp->next;
        }
        // Kembalikan vector berisi semua lokasi asal item
        return res;
    }

    // Fungsi untuk mendapatkan daftar nama dari semua item dalam keranjang
    std::vector<std::string> getAllNames() {
        // Buat vector kosong untuk menyimpan nama item
        std::vector<std::string> res;
        // Traverse seluruh linked list dari head hingga akhir
        CartNode* temp = head;
        while (temp) {
            // Tambahkan nama item ke vector
            res.push_back(temp->name);
            // Geser pointer ke node berikutnya
            temp = temp->next;
        }
        // Kembalikan vector berisi semua nama item
        return res;
    }

    // Fungsi untuk mendapatkan daftar harga dari semua item dalam keranjang
    std::vector<long> getAllPrices() {
        // Buat vector kosong untuk menyimpan harga item
        std::vector<long> res;
        // Traverse seluruh linked list dari head hingga akhir
        CartNode* temp = head;
        while (temp) {
            // Tambahkan harga item ke vector
            res.push_back(temp->price);
            // Geser pointer ke node berikutnya
            temp = temp->next;
        }
        // Kembalikan vector berisi semua harga item
        return res;
    }
};

// Akhir guard preprocessor: menutup pencegahan include ganda
#endif // CART_LIST_H