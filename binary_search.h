// Guard preprocessor: Memastikan file header ini hanya di-include satu kali dalam kompilasi
#ifndef BINARY_SEARCH_H
// Mendefinisikan macro BINARY_SEARCH_H sebagai penanda bahwa file ini sudah dimuat
#define BINARY_SEARCH_H

// Mengunduh komponen library std::string untuk memanipulasi tipe data teks/string
#include <string>
// Mengunduh komponen library std::vector untuk menggunakan array dinamis (resizable)
#include <vector>
// Mengunduh library algorithm untuk fungsi sorting (std::sort) dan manipulasi data (std::transform)
#include <algorithm>
// Mengunduh library cctype untuk fungsi pemrosesan karakter seperti mengubah ke huruf kecil (tolower)
#include <cctype>

// Struktur data (blueprint) untuk menyimpan informasi lengkap dari setiap satu item produk
struct ProductEntry {
    int         id;        // Menyimpan ID unik produk berupa angka integer
    std::string name;      // Menyimpan nama lengkap produk
    std::string category;  // Menyimpan kategori kelompok produk
    long        price;     // Menyimpan harga produk menggunakan tipe data long (angka besar)
    std::string origin;    // Menyimpan negara atau tempat asal produk dibuat
};

// Kelas utama yang menangani semua logika algoritma Binary Search produk
class BinarySearch {
private:
    std::vector<ProductEntry> sortedByName; // Container/array produk yang nantinya diurutkan berdasarkan Nama (A-Z)
    std::vector<ProductEntry> sortedById;   // Container/array produk yang nantinya diurutkan berdasarkan ID (Kecil-Besar)
    bool isBuilt = false;                   // Flag/penanda apakah indeks array di atas sudah dalam posisi terurut atau belum

    // Fungsi internal untuk mengubah semua huruf dalam string menjadi huruf kecil (case-insensitive helper)
    static std::string toLower(std::string s) {
        // Mengubah string dari karakter pertama sampai akhir menjadi huruf kecil secara inplace
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        // Mengembalikan string yang sudah diubah total menjadi huruf kecil
        return s;
    }

public:
    // Fungsi untuk memasukkan produk baru satu per satu ke dalam daftar tampungan
    void addProduct(int id, const std::string& name,
                    const std::string& category, long price,
                    const std::string& origin) {
        // Membuat objek baru berbentuk ProductEntry langsung dari parameter fungsi
        ProductEntry e{id, name, category, price, origin};
        // Memasukkan objek produk baru tersebut ke dalam array tampungan Nama
        sortedByName.push_back(e);
        // Memasukkan objek produk baru tersebut ke dalam array tampungan ID
        sortedById.push_back(e);
        // Set flag menjadi false karena ada data baru masuk, sehingga susunan data belum terurut kembali
        isBuilt = false; 
    }

    // Fungsi wajib untuk mengurutkan (sorting) data sebelum proses pencarian binary search dimulai
    void buildIndex() {
        // Mengurutkan array sortedByName menggunakan algoritma quicksort/introsort bawaan C++
        std::sort(sortedByName.begin(), sortedByName.end(),
                  // Fungsi lambda (anonim) untuk membandingkan nama produk secara alfabetis (huruf kecil)
                  [](const ProductEntry& a, const ProductEntry& b) {
                      return toLower(a.name) < toLower(b.name);
                  });
        // Mengurutkan array sortedById secara ascending (dari ID terkecil ke terbesar)
        std::sort(sortedById.begin(), sortedById.end(),
                  // Fungsi lambda untuk membandingkan ID produk secara numerik
                  [](const ProductEntry& a, const ProductEntry& b) {
                      return a.id < b.id;
                  });
        // Mengubah status flag menjadi true, menandakan data siap dicari dengan Binary Search
        isBuilt = true;
    }

    // Fungsi Binary Search untuk mencari produk berdasarkan ID secara eksak (pasti)
    const ProductEntry* searchById(int targetId) {
        // Cek pengaman: jika user lupa memanggil buildIndex(), maka otomatis jalankan pengurutan dahulu
        if (!isBuilt) buildIndex();

        // Menginisialisasi pointer batas kiri (lo) di indeks ke-0 dan batas kanan (hi) di indeks terakhir
        int lo = 0, hi = static_cast<int>(sortedById.size()) - 1;
        // Melakukan perulangan selama rentang pencarian masih valid (batas kiri tidak melewati kanan)
        while (lo <= hi) {
            // Mencari titik tengah (mid) dengan formula aman terhindar dari integer overflow
            int mid = lo + (hi - lo) / 2;
            // Jika ID produk di titik tengah sama persis dengan ID yang dicari
            if (sortedById[mid].id == targetId) {
                // Mengembalikan alamat memori (pointer) dari produk yang ditemukan tersebut
                return &sortedById[mid];
            // Jika ID di titik tengah ternyata lebih kecil dari ID target
            } else if (sortedById[mid].id < targetId) {
                // Buang setengah data kiri, geser batas kiri ke kanan setelah mid
                lo = mid + 1;
            // Jika ID di titik tengah ternyata lebih besar dari ID target
            } else {
                // Buang setengah data kanan, geser batas kanan ke kiri sebelum mid
                hi = mid - 1;
            }
        }
        // Mengembalikan nilai kosong (nullptr) jika perulangan selesai dan produk tidak ditemukan
        return nullptr;
    }

    // Fungsi Binary Search untuk mencari produk berdasarkan awalan nama (Prefix Matching)
    std::vector<ProductEntry> searchByNamePrefix(const std::string& keyword) {
        // Cek pengaman: otomatis urutkan indeks jika data belum siap
        if (!isBuilt) buildIndex();

        // Mengubah keyword pencarian menjadi huruf kecil agar pencarian bersifat tidak sensitif huruf kapital
        std::string kw = toLower(keyword);
        // Menyiapkan vector kosong untuk menampung semua produk yang cocok dengan awalan tersebut
        std::vector<ProductEntry> result;

        // Inisialisasi batas awal pencarian biner
        int lo = 0, hi = static_cast<int>(sortedByName.size()) - 1;
        // Menentukan nilai default batas bawah pada posisi di luar jangkauan array (dianggap tidak ada)
        int lowerBound = static_cast<int>(sortedByName.size()); 
        // Perulangan biner untuk mencari posisi pertama (indeks paling kiri) yang >= keyword
        while (lo <= hi) {
            // Menentukan posisi tengah array
            int mid = lo + (hi - lo) / 2;
            // Mengubah nama produk di posisi tengah menjadi huruf kecil
            std::string midLow = toLower(sortedByName[mid].name);
            // Jika nama produk di posisi tengah secara alfabetis bernilai lebih besar atau sama dengan keyword
            if (midLow >= kw) {
                // Catat posisi indeks ini sebagai kandidat batas bawah pencarian
                lowerBound = mid;
                // Coba persempit wilayah ke arah kiri lagi untuk mencari tahu apakah ada yang lebih awal
                hi = mid - 1;
            // Jika nama produk di posisi tengah secara alfabetis bernilai lebih kecil dari keyword
            } else {
                // Alihkan pencarian ke wilayah kanan (geser batas kiri)
                lo = mid + 1;
            }
        }

        // Melakukan iterasi linier mulai dari indeks lowerBound yang sudah ditemukan ke arah kanan
        for (int i = lowerBound; i < (int)sortedByName.size(); ++i) {
            // Mengubah nama produk pada indeks berjalan menjadi huruf kecil
            std::string nameLow = toLower(sortedByName[i].name);
            // Memotong string nama dari indeks ke-0 sepanjang ukuran keyword dan mencocokkannya
            if (nameLow.substr(0, kw.size()) == kw) {
                // Jika potongannya persis sama (berarti awalan cocok), masukkan produk ke dalam hasil pencarian
                result.push_back(sortedByName[i]);
            } else {
                // Karena data sudah terurut alfabetis, sekali saja tidak cocok berarti ke bawahnya pasti tidak cocok (break)
                break;
            }
        }
        // Mengembalikan daftar kumpulan produk yang berhasil lolos seleksi awalan kata
        return result;
    }

    // Fungsi edukasional untuk menghasilkan log teks penjelasan proses langkah demi langkah Binary Search ID
    std::string getSearchLog(int targetId) {
        // Cek pengaman: memastikan data terurut sebelum log dicatat
        if (!isBuilt) buildIndex();

        // Membuat string penampung teks log diawali dengan judul log pencarian
        std::string log = "=== BINARY SEARCH LOG (by ID: " + std::to_string(targetId) + ") ===\n";
        // Mengatur posisi awal indeks kiri dan kanan untuk simulasi tracking pencarian
        int lo = 0, hi = static_cast<int>(sortedById.size()) - 1;
        // Variabel counter untuk mencatat jumlah iterasi perulangan yang dilakukan
        int iterasi = 1;

        // Mulai simulasi perulangan biner yang sama dengan fungsi pencarian ID asli
        while (lo <= hi) {
            // Menghitung posisi tengah indeks
            int mid = lo + (hi - lo) / 2;
            // Menyusun teks log yang menginfokan posisi variabel lo, mid, hi saat ini, beserta ID produk yang sedang dicek
            log += "Iterasi " + std::to_string(iterasi++) +
                   ": lo=" + std::to_string(lo) +
                   " mid=" + std::to_string(mid) +
                   " hi=" + std::to_string(hi) +
                   " → cek ID " + std::to_string(sortedById[mid].id);
            // Logika cek: jika ID produk di posisi tengah adalah target yang dicari
            if (sortedById[mid].id == targetId) {
                // Menambahkan teks sukses pada log dan menghentikan perulangan (break)
                log += " ✅ DITEMUKAN!\n";
                break;
            // Logika cek: jika ID produk di posisi tengah lebih kecil dari target
            } else if (sortedById[mid].id < targetId) {
                // Menambahkan informasi arah pergeseran log ke kanan
                log += " → geser lo ke kanan\n";
                // Menggeser nilai batas kiri
                lo = mid + 1;
            // Logika cek: jika ID produk di posisi tengah lebih besar dari target
            } else {
                // Menambahkan informasi arah pergeseran log ke kiri
                log += " → geser hi ke kiri\n";
                // Menggeser nilai batas kanan
                hi = mid - 1;
            }
        }
        // Jika loop selesai dan lo > hi artinya data benar-benar tidak ada di array, catat di log
        if (lo > hi) log += "❌ Produk tidak ditemukan.\n";
        // Menambahkan kesimpulan akhir berupa jumlah total perbandingan operasi dibanding ukuran data asli
        log += "Total perbandingan: " + std::to_string(iterasi - 1) +
               " (dari " + std::to_string(sortedById.size()) + " produk)";
        // Mengembalikan string rekaman log utuh ke pemanggil fungsi
        return log;
    }

    // Fungsi untuk mengosongkan/membersihkan seluruh database internal produk yang tersimpan
    void clear() {
        // Menghapus total isi dari array sortedByName
        sortedByName.clear();
        // Menghapus total isi dari array sortedById
        sortedById.clear();
        // Mengembalikan flag status ke false karena tidak ada data yang terindeks lagi
        isBuilt = false;
    }

    // Fungsi konstan (tidak mengubah data) untuk mendapatkan jumlah total produk saat ini
    int count() const {
        // Mengembalikan ukuran size dari array (menggunakan sortedById sebagai acuan jumlah)
        return static_cast<int>(sortedById.size());
    }
};

// Menutup blok pembungkus guard preprocessor #ifndef BINARY_SEARCH_H
#endif // BINARY_SEARCH_H