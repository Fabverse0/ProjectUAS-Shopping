// Guard preprocessor: Mencegah error duplikasi jika file header ini di-include lebih dari satu kali
#ifndef BST_H
// Mendefinisikan macro BST_H sebagai penanda bahwa file ini sudah dimuat ke sistem kompiler
#define BST_H

// Memuat library std::string untuk menggunakan fungsi manipulasi teks (nama, kategori, asal produk)
#include <string>
// Memuat library std::vector untuk menampung hasil penelusuran (traversal) berupa array dinamis
#include <vector>

// =========================================================
// BINARY SEARCH TREE (BST) — Tampilan Katalog Urut Harga
// =========================================================

// Struktur data untuk sebuah Node (simpul) di dalam struktur pohon Binary Search Tree
struct BSTNode {
    int         id;        // Menyimpan data ID unik produk
    std::string name;      // Menyimpan data nama produk
    std::string category;  // Menyimpan data kategori produk
    long        price;     // Menyimpan data harga produk (menjadi acuan utama penyusunan pohon/key)
    std::string origin;    // Menyimpan data asal negara produk
    BSTNode* left;      // Pointer (penunjuk) ke anak cabang sebelah kiri (harga lebih kecil)
    BSTNode* right;     // Pointer (penunjuk) ke anak cabang sebelah kanan (harga lebih besar/sama dengan)

    // Constructor untuk menginisialisasi objek Node baru langsung dengan nilai-nilai produk
    BSTNode(int i, std::string n, std::string c, long p, std::string o)
        : id(i), name(n), category(c), price(p), origin(o), // Mengisi properti sesuai parameter masukan
          left(nullptr), right(nullptr) {} // Mengatur pointer anak kiri dan kanan ke kosong (nullptr) pada awal dibuat
};

// Kelas utama yang mengatur manajemen data dan operasi struktur Binary Search Tree
class PriceBST {
private:
    BSTNode* root; // Pointer utama yang menunjuk ke gerbang masuk paling atas pohon (akar/root)

    // Fungsi internal (rekursif) untuk menempatkan node produk baru pada posisi yang tepat di dalam pohon
    BSTNode* insertRek(BSTNode* node, int id, const std::string& name,
                       const std::string& cat, long price, const std::string& origin) {
        // Jika posisi saat ini kosong/belum ada node, buat node baru di tempat ini dan kembalikan posisinya
        if (node == nullptr) {
            return new BSTNode(id, name, cat, price, origin);
        }
        // Jika harga produk baru lebih kecil dari harga node saat ini, arahkan pencarian tempat ke cabang kiri
        if (price < node->price) {
            node->left  = insertRek(node->left,  id, name, cat, price, origin);
        } 
        // Jika harga produk baru lebih besar atau sama dengan harga node saat ini, arahkan ke cabang kanan
        else {
            node->right = insertRek(node->right, id, name, cat, price, origin);
        }
        // Mengembalikan pointer node saat ini setelah proses penyisipan rekursif selesai di bawahnya
        return node;
    }

    // Fungsi internal (rekursif) untuk menelusuri pohon secara In-Order (Kiri -> Root -> Kanan)
    void inOrderRek(BSTNode* node, std::vector<BSTNode*>& result) const {
        // Basis rekursif: Jika node kosong, hentikan penelusuran (kembali ke tumpukan fungsi sebelumnya)
        if (node == nullptr) return;
        // Telusuri secara mendalam ke semua cabang kiri terlebih dahulu (mengambil harga terkecil)
        inOrderRek(node->left, result);
        // Setelah cabang kiri habis, masukkan alamat node saat ini ke dalam array hasil pencatatan
        result.push_back(node);
        // Lanjutkan penelusuran ke cabang kanan untuk mengambil data yang nilainya lebih besar
        inOrderRek(node->right, result);
    }

    // Fungsi internal (rekursif) untuk menelusuri pohon secara Terbalik (Kanan -> Root -> Kiri)
    void reverseOrderRek(BSTNode* node, std::vector<BSTNode*>& result) const {
        // Basis rekursif: Jika node kosong, keluar dari fungsi
        if (node == nullptr) return;
        // Telusuri secara mendalam ke semua cabang kanan terlebih dahulu (mengambil harga terbesar)
        reverseOrderRek(node->right, result);
        // Masukkan alamat node saat ini ke dalam array hasil setelah cabang kanan selesai dieksplorasi
        result.push_back(node);
        // Lanjutkan penelusuran ke cabang kiri untuk mengambil data yang nilainya lebih kecil
        reverseOrderRek(node->left, result);
    }

    // Fungsi internal (rekursif) untuk menghapus seluruh node dari memori dari bawah ke atas (Post-Order)
    void clearRek(BSTNode* node) {
        // Jika node memang sudah kosong, tidak ada yang perlu dihapus lagi
        if (node == nullptr) return;
        // Hapus semua anak cabang di sebelah kiri terlebih dahulu secara rekursif
        clearRek(node->left);
        // Hapus semua anak cabang di sebelah kanan secara rekursif
        clearRek(node->right);
        // Setelah kedua cabang anak bersih, hapus (deallocate) objek node saat ini dari memori RAM
        delete node;
    }

    // Fungsi internal (rekursif) untuk menyusun bagan teks visualisasi struktur pohon BST
    void printTreeRek(BSTNode* node, std::string prefix,
                      bool isLeft, std::string& out) const {
        // Jika cabang tidak memiliki node, tidak ada teks yang perlu ditambahkan
        if (node == nullptr) return;
        // Membuat baris baru dengan menambahkan teks prefix dan simbol cabang sesuai status (kiri/kanan)
        out += prefix + (isLeft ? "├── " : "└── ");
        // Menampilkan data harga beserta nama produk pada baris visualisasi tersebut
        out += "[" + std::to_string(node->price) + "] " + node->name + "\n";
        // Panggil fungsi secara rekursif untuk anak cabang kiri dengan memperbarui karakter garis lurus prefix
        printTreeRek(node->left,  prefix + (isLeft ? "│   " : "    "), true,  out);
        // Panggil fungsi secara rekursif untuk anak cabang kanan dengan memperbarui karakter garis lurus prefix
        printTreeRek(node->right, prefix + (isLeft ? "│   " : "    "), false, out);
    }

public:
    // Constructor publik: Membuat objek PriceBST baru dengan mengatur root awal ke kosong (nullptr)
    PriceBST() : root(nullptr) {}

    // Destructor publik: Otomatis dipanggil saat objek dihancurkan untuk mencegah kebocoran memori (memory leak)
    ~PriceBST() {
        // Memanggil helper fungsi pembersih rekursif mulai dari node root paling atas
        clearRek(root);
    }

    // Antarmuka publik untuk memasukkan produk baru ke dalam pohon
    void insert(int id, const std::string& name, const std::string& cat,
                long price, const std::string& origin) {
        // Memperbarui pointer root utama dengan hasil penataan fungsi rekursif insertRek
        root = insertRek(root, id, name, cat, price, origin);
    }

    // Antarmuka publik untuk mendapatkan seluruh data produk berurutan dari Termurah ke Termahal
    std::vector<BSTNode*> getSortedAscending() const {
        // Menyiapkan kontainer vector kosong untuk menampung alamat pointer node produk
        std::vector<BSTNode*> result;
        // Mengisi kontainer menggunakan fungsi penelusuran inOrderRek mulai dari root
        inOrderRek(root, result);
        // Mengembalikan daftar produk yang sekarang susunannya sudah terurut menaik (ascending)
        return result;
    }

    // Antarmuka publik untuk mendapatkan seluruh data produk berurutan dari Termahal ke Termurah
    std::vector<BSTNode*> getSortedDescending() const {
        // Menyiapkan kontainer vector kosong untuk menampung alamat pointer node produk
        std::vector<BSTNode*> result;
        // Mengisi kontainer menggunakan fungsi penelusuran terbalik reverseOrderRek mulai dari root
        reverseOrderRek(root, result);
        // Mengembalikan daftar produk yang sekarang susunannya sudah terurut menurun (descending)
        return result;
    }

    // Antarmuka publik untuk mengecek status keterisian pohon BST
    bool isEmpty() const { return root == nullptr; } // Mengembalikan nilai true jika root bernilai kosong (nullptr)

    // Antarmuka publik untuk mereset ulang seluruh pohon menjadi kosong kembali
    void clear() {
        // Menghapus semua node teralokasi di dalam memori
        clearRek(root);
        // Mengembalikan posisi root ke status awal (nullptr/kosong) sehingga siap digunakan lagi
        root = nullptr;
    }

    // Antarmuka publik untuk mengambil visualisasi bagan hierarki pohon dalam bentuk teks string (untuk UI)
    std::string getTreeVisualization() const {
        // Jika pohon tidak memiliki data sama sekali, langsung kembalikan pesan pemberitahuan singkat
        if (root == nullptr) return "(BST kosong)";
        // Menyiapkan teks pembuka judul visualisasi bagan pohon
        std::string out = "=== STRUKTUR BST (berdasarkan Harga) ===\n";
        // Mencatat info data yang menempati posisi puncak pohon (Root utama)
        out += "Root: [" + std::to_string(root->price) + "] " + root->name + "\n";
        // Menelusuri rekursif cabang sebelah kiri root untuk digambar ke dalam string
        printTreeRek(root->left,  "", true,  out);
        // Menelusuri rekursif cabang sebelah kanan root untuk digambar ke dalam string
        printTreeRek(root->right, "", false, out);
        // Menambahkan teks petunjuk legenda pembacaan arah cabang pohon di bagian paling bawah
        out += "\n[Kiri = harga lebih kecil | Kanan = harga lebih besar]";
        // Mengembalikan teks visualisasi pohon yang sudah rapi terangkai
        return out;
    }
};

// Menutup batas pengecekan guard preprocessor dari simbol BST_H di awal file tadi
#endif // BST_H