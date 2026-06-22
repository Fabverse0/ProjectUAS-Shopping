// Guard preprocessor untuk mencegah include ganda pada file header
#ifndef NAV_STACK_H
// Definisikan simbol NAV_STACK_H sebagai tanda file sudah di-include
#define NAV_STACK_H

// =========================================================
// STRUKTUR DATA: STACK (Riwayat Navigasi Halaman)
// Implementasi berbasis Linked List (bukan array)
// Push  = saat user pindah halaman
// Pop   = saat user menekan tombol Back
// Peek  = melihat halaman sebelumnya tanpa keluar dari stack
// =========================================================

// Definisikan struct NavNode untuk mewakili satu node dalam stack navigasi
struct NavNode {
    // Menyimpan index halaman dalam QStackedWidget
    int pageIndex;
    // Menyimpan label/nama halaman untuk ditampilkan (misal: "Katalog", "Keranjang")
    std::string pageLabel;
    // Pointer ke node berikutnya dalam linked list stack
    NavNode* next;

    // Konstruktor untuk membuat node baru dengan index dan label halaman
    NavNode(int idx, std::string label)
        // Inisialisasi member pageIndex dengan parameter idx
        : pageIndex(idx),
        // Inisialisasi member pageLabel dengan parameter label
        pageLabel(label),
        // Inisialisasi member next dengan nullptr (belum ada node berikutnya)
        next(nullptr) {}
};

// Definisikan class NavStack untuk merepresentasikan stack riwayat navigasi
class NavStack {
private:
    // Pointer ke node teratas stack (top of stack)
    NavNode* top;
    // Menyimpan jumlah node yang ada di dalam stack saat ini
    int stackSize;

public:
    // Konstruktor: inisialisasi stack kosong
    NavStack() : top(nullptr), stackSize(0) {}

    // Destruktor: hapus semua node saat object dihancurkan
    ~NavStack() {
        // Selama stack tidak kosong, lakukan pop hingga kosong
        while (!isEmpty()) pop();
    }

    // Fungsi untuk mengecek apakah stack kosong (tidak ada halaman dalam riwayat)
    bool isEmpty() const {
        // Kembalikan true jika top adalah nullptr (kosong), false jika ada node
        return top == nullptr;
    }

    // Fungsi untuk mendapatkan jumlah node/halaman yang ada di stack
    int size() const {
        // Kembalikan nilai stackSize yang menyimpan jumlah elemen
        return stackSize;
    }

    // Fungsi Push: catat halaman saat ini sebelum user berpindah ke halaman lain
    void push(int pageIndex, const std::string& label) {
        // Buat node baru dengan pageIndex dan label yang diberikan
        NavNode* newNode = new NavNode(pageIndex, label);
        // Sambungkan node baru ke top stack (node baru menjadi top baru)
        newNode->next = top;
        // Update pointer top untuk menunjuk ke node baru
        top = newNode;
        // Tambah counter stackSize untuk mencatat penambahan satu node
        stackSize++;
    }

    // Fungsi Pop: ambil halaman terakhir yang dikunjungi (untuk fitur Back)
    int pop() {
        // Jika stack kosong, kembalikan -1 sebagai indikator error
        if (isEmpty()) return -1;
        // Simpan pointer node teratas dalam variable temp
        NavNode* temp = top;
        // Ambil nilai pageIndex dari node yang akan dihapus
        int idx = temp->pageIndex;
        // Update top untuk menunjuk ke node berikutnya (remove node lama)
        top = top->next;
        // Hapus node yang sudah di-pop dari memori
        delete temp;
        // Kurangi counter stackSize karena satu node sudah dihapus
        stackSize--;
        // Kembalikan pageIndex yang di-pop
        return idx;
    }

    // Fungsi Peek: lihat pageIndex halaman teratas tanpa menghapusnya (pop)
    int peek() const {
        // Jika stack kosong, kembalikan -1 sebagai indikator error
        if (isEmpty()) return -1;
        // Kembalikan pageIndex dari node teratas stack
        return top->pageIndex;
    }

    // Fungsi peekLabel: lihat label halaman teratas tanpa menghapusnya
    std::string peekLabel() const {
        // Jika stack kosong, kembalikan string kosong ""
        if (isEmpty()) return "";
        // Kembalikan pageLabel (nama halaman) dari node teratas stack
        return top->pageLabel;
    }

    // Fungsi untuk debugging: tampilkan seluruh isi stack dalam bentuk list
    std::vector<std::string> getHistoryList() const {
        // Buat vector kosong untuk menyimpan informasi setiap halaman
        std::vector<std::string> history;
        // Inisialisasi pointer curr untuk traverse dari top
        NavNode* curr = top;
        // Selama curr tidak nullptr (masih ada node di stack)
        while (curr != nullptr) {
            // Tambahkan informasi label dan index node saat ini ke vector
            history.push_back(curr->pageLabel + " [idx:" + std::to_string(curr->pageIndex) + "]");
            // Geser pointer curr ke node berikutnya
            curr = curr->next;
        }
        // Kembalikan vector berisi seluruh riwayat navigasi
        return history;
    }

    // Fungsi untuk mengosongkan stack (misal saat user logout)
    void clear() {
        // Selama stack tidak kosong, lakukan pop hingga kosong
        while (!isEmpty()) pop();
    }
};

// Akhir guard preprocessor: menutup penokohan include ganda
#endif // NAV_STACK_H