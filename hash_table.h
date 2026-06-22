// Guard preprocessor: Mencegah duplikasi deklarasi file header saat proses kompilasi
#ifndef HASH_TABLE_H
// Mendefinisikan simbol HASH_TABLE_H sebagai penanda file ini sudah dimuat
#define HASH_TABLE_H

// Memuat library std::string untuk memproses data teks (seperti nama produk, kode voucher, dan ID order)
#include <string>
// Memuat library std::vector untuk membuat dynamic array sebagai wadah utama bucket
#include <vector>
// Memuat library std::functional untuk menyediakan dukungan fungsi bawaan jika diperlukan
#include <functional>

// =========================================================
// HASH TABLE — Quick Lookup O(1) Average
// =========================================================


// =========================================================
// 1. PRODUCT HASH TABLE — Key: int (Product ID)
// =========================================================

// Struktur node untuk menyimpan data produk sekaligus bertindak sebagai elemen Linked List (Separate Chaining)
struct ProductHashEntry {
    int         id;        // Menyimpan kunci pencarian (Key) berupa ID unik produk
    std::string name;      // Menyimpan nama lengkap produk
    std::string category;  // Menyimpan kategori produk
    long        price;     // Menyimpan harga produk
    std::string origin;    // Menyimpan negara/lokasi asal produk
    ProductHashEntry* next = nullptr; // Pointer ke node berikutnya jika terjadi tabrakan data (collision)

    // Constructor untuk menginisialisasi properti entri produk baru
    ProductHashEntry(int i, const std::string& n, const std::string& c,
                     long p, const std::string& o)
        : id(i), name(n), category(c), price(p), origin(o), next(nullptr) {} // Data awal diset, next diatur kosong
};

// Kelas untuk mengelola tabel hash data produk
class ProductHashTable {
private:
    static const int DEFAULT_SIZE = 16;       // Menentukan kapasitas ukuran awal bucket tabel hash (harus pangkat 2)
    std::vector<ProductHashEntry*> buckets;   // Array dinamis penampung pointer kepala (head) dari Linked List produk
    int tableSize;                            // Menyimpan informasi kapasitas total bucket saat ini
    int itemCount;                            // Menyimpan jumlah total item produk yang sukses tersimpan

    // Fungsi internal untuk menghitung indeks bucket menggunakan metode Fibonacci Hashing
    int hashFn(int key) const {
        // Mengalikan key dengan konstanta emas Fibonacci, melakukan bitwise shift, dan dimodulo ukuran tabel
        return ((key * 2654435769U) >> (32 - __builtin_ctz(tableSize))) % tableSize;
    }

    // Fungsi internal untuk memperbesar ukuran tabel dan menata ulang posisi data (re-indexing)
    void rehash() {
        int newSize = tableSize * 2; // Menggandakan kapasitas ukuran tabel baru (misal dari 16 menjadi 32)
        std::vector<ProductHashEntry*> newBuckets(newSize, nullptr); // Menyiapkan wadah bucket baru yang kosong

        // Melakukan penelusuran (looping) ke setiap indeks bucket pada tabel lama
        for (int i = 0; i < tableSize; ++i) {
            ProductHashEntry* curr = buckets[i]; // Membuka pointer kepala chain pada indeks ke-i
            // Menelusuri seluruh linked list yang ada pada bucket tersebut
            while (curr) {
                ProductHashEntry* next = curr->next; // Mengamankan alamat node berikutnya sebelum memutus rantai
                // Menghitung ulang indeks bucket baru untuk node saat ini menggunakan kapasitas newSize
                int newIdx = ((curr->id * 2654435769U) >> (32 - __builtin_ctz(newSize))) % newSize;
                curr->next = newBuckets[newIdx]; // Memasang node ke kepala rantai baru pada bucket tujuan
                newBuckets[newIdx] = curr;       // Memperbarui pointer utama kepala bucket baru
                curr = next;                     // Pindah ke node berikutnya dari rantai tabel lama
            }
        }
        buckets   = newBuckets; // Mengganti kontainer lama dengan kontainer baru yang berukuran besar
        tableSize = newSize;    // Memperbarui informasi kapasitas total tabel produk
    }

public:
    // Constructor: Mempersiapkan tabel dengan ukuran default (16), diisi pointer kosong, dan total item nol
    ProductHashTable()
        : buckets(DEFAULT_SIZE, nullptr), tableSize(DEFAULT_SIZE), itemCount(0) {}

    // Destructor: Otomatis dipanggil untuk membersihkan sisa alokasi memori saat objek dihancurkan
    ~ProductHashTable() { clear(); }

    // Fungsi untuk memasukkan produk baru atau memperbarui data jika ID sudah terdaftar
    void insert(int id, const std::string& name, const std::string& category,
                long price, const std::string& origin) {
        int idx = hashFn(id); // Menghitung indeks bucket target berdasarkan ID produk
        ProductHashEntry* curr = buckets[idx]; // Menunjuk ke awal mata rantai list pada indeks tersebut
        // Loop mencari apakah ID produk tersebut sebenarnya sudah eksis di dalam sistem
        while (curr) {
            if (curr->id == id) { // Jika ID ditemukan sama
                curr->name = name; curr->category = category; // Melakukan pembaruan (update) seluruh informasi produk
                curr->price = price; curr->origin = origin;   // baris per baris data produk
                return; // Keluar dari fungsi karena misi update telah selesai
            }
            curr = curr->next; // Geser ke simpul linked list berikutnya
        }

        // Jika ID belum ada, buat objek entri produk baru di memori heap
        ProductHashEntry* newEntry = new ProductHashEntry(id, name, category, price, origin);
        newEntry->next = buckets[idx]; // Menyambungkan ekor entri baru ke kepala rantai lama (Insert di Depan)
        buckets[idx]   = newEntry;    // Menetapkan entri baru ini sebagai kepala rantai yang baru di bucket
        itemCount++; // Menambah catatan jumlah total data produk

        // Memeriksa batasan Load Factor: jika jumlah item melampaui 75% kapasitas, lakukan rehash otomatis
        if (itemCount > tableSize * 3 / 4) rehash();
    }

    // Fungsi untuk mencari informasi produk berdasarkan ID secara cepat
    const ProductHashEntry* lookup(int id) const {
        int idx = hashFn(id); // Menghitung indeks bucket lokasi produk berada
        ProductHashEntry* curr = buckets[idx]; // Mulai membaca dari kepala rantai linked list di bucket tersebut
        // Menelusuri rantai data produk pada bucket bersangkutan
        while (curr) {
            if (curr->id == id) return curr; // Jika ID cocok, kembalikan alamat memori objek produk tersebut
            curr = curr->next; // Pindah ke simpul linked list berikutnya
        }
        return nullptr; // Mengembalikan data kosong jika produk tidak ditemukan di sepanjang rantai
    }

    // Fungsi untuk menghapus permanen data produk berdasarkan ID dari tabel hash
    void remove(int id) {
        int idx = hashFn(id); // Menemukan posisi indeks bucket target produk
        ProductHashEntry* curr = buckets[idx]; // Pointer untuk menelusuri node saat ini
        ProductHashEntry* prev = nullptr;      // Pointer untuk mencatat node sebelum posisi curr (untuk menyambung rantai)
        // Proses pencarian node produk yang ingin dieliminasi
        while (curr) {
            if (curr->id == id) { // Ketika node target klop dengan ID yang dicari
                if (prev) prev->next = curr->next; // Jika posisi di tengah/belakang, sambungkan node sebelum ke sesudah curr
                else       buckets[idx] = curr->next; // Jika di kepala, alihkan pointer utama direktori bucket ke node sesudahnya
                delete curr; // Membebaskan memori RAM yang digunakan oleh objek produk tersebut
                itemCount--; // Mengurangi catatan jumlah total item produk aktif
                return; // Selesai menghapus, keluar dari fungsi
            }
            prev = curr;       // Geser pointer pembantu prev ke posisi curr saat ini
            curr = curr->next; // Geser pointer utama curr ke simpul berikutnya
        }
    }

    // Fungsi total pembersihan untuk menghapus seluruh node produk tanpa menyisakan memory leak
    void clear() {
        for (int i = 0; i < tableSize; ++i) { // Membuka baris indeks bucket satu per satu
            ProductHashEntry* curr = buckets[i]; // Memulai dari kepala linked list bucket ke-i
            while (curr) { // Selama mata rantai masih berisi objek produk
                ProductHashEntry* next = curr->next; // Amankan alamat node sesudahnya
                delete curr; // Hapus objek produk saat ini
                curr = next; // Pindah posisi ke node berikutnya yang diamankan tadi
            }
            buckets[i] = nullptr; // Setel ulang kepala bucket menjadi kosong total
        }
        itemCount = 0; // Mengembalikan indikator jumlah total item ke posisi nol
    }

    // Kelompok fungsi konstan aksesor untuk mengambil informasi status statistik internal tabel
    int  count()     const { return itemCount; } // Mengambil kuantitas item terdaftar
    int  size()      const { return tableSize; } // Mengambil total ruang slot bucket yang tersedia
    double loadFactor() const { return itemCount / (double)tableSize; } // Menghitung rasio kepadatan isi tabel hash

    // Fungsi untuk merangkai string ringkasan data statistik performa tabel (untuk visualisasi sistem)
    std::string getStatsString() const {
        int used = 0, maxChain = 0, collisions = 0; // Inisialisasi variabel penghitung kualitas distribusi data
        for (int i = 0; i < tableSize; ++i) { // Iterasi meneliti seluruh bucket yang ada
            if (!buckets[i]) continue; // Jika kosong, lewati ke bucket berikutnya
            used++; // Tambah total hitungan jumlah bucket yang berhasil terisi
            int chain = 0; // Variabel penghitung panjang rantai linked list di dalam satu bucket
            for (ProductHashEntry* c = buckets[i]; c; c = c->next) chain++; // Menghitung total node di sepanjang rantai
            if (chain > 1) collisions += chain - 1; // Jika isi rantai > 1, berarti telah terjadi tabrakan indeks data (collision)
            if (chain > maxChain) maxChain = chain; // Mencatat rekor rantai terpanjang yang terbentuk dalam tabel
        }
        // Mengembalikan string gabungan hasil analisis kalkulasi statistik distribusi performa pencarian produk
        return "=== PRODUCT HASH TABLE STATS ===\n"
               "Table Size    : " + std::to_string(tableSize) + " bucket\n"
               "Items         : " + std::to_string(itemCount) + "\n"
               "Load Factor   : " + std::to_string(loadFactor()).substr(0,4) + "\n"
               "Buckets Used  : " + std::to_string(used) + "\n"
               "Max Chain Len : " + std::to_string(maxChain) + "\n"
               "Collisions    : " + std::to_string(collisions) + "\n"
               " [O(1) avg lookup — rehash otomatis jika LF > 0.75]";
    }
};


// =========================================================
// 2. VOUCHER HASH TABLE — Key: string (kode voucher)
// =========================================================

// Struktur entri data informasi voucher diskon belanja
struct VoucherEntry {
    std::string kode;        // Kunci pencarian teks (Key) berupa kode unik voucher (misal: "DISKON50")
    int         persen;      // Nilai besaran persentase potongan diskon
    std::string deskripsi;   // Keterangan atau deskripsi dari voucher promo
    bool        aktif;       // Status validitas voucher (aktif = true, hangus/nonaktif = false)
    VoucherEntry* next = nullptr; // Pointer penunjuk node tabrakan berantai berikutnya

    // Constructor pengisian field data objek voucher baru
    VoucherEntry(const std::string& k, int p, const std::string& d, bool a = true)
        : kode(k), persen(p), deskripsi(d), aktif(a), next(nullptr) {}
};

// Kelas manajemen penyimpanan data voucher belanja menggunakan algoritma hash map
class VoucherHashTable {
private:
    static const int DEFAULT_SIZE = 8; // Kapasitas awal direktori bucket voucher sengaja diset lebih kecil (8 slot)
    std::vector<VoucherEntry*> buckets; // Container array dinamis untuk menampung barisan head linked list voucher
    int tableSize;                     // Menyimpan kapasitas total slot penampung saat ini
    int itemCount;                     // Menyimpan kuantitas item voucher terdaftar yang aktif/pernah dimasukkan

    // Algoritma standard fungsi hash djb2 — Sangat efisien mereduksi kolisi pada teks string pendek
    int hashFn(const std::string& key) const {
        unsigned long h = 5381; // Nilai inisialisasi awal angka prima generator djb2
        for (char c : key) h = ((h << 5) + h) + (unsigned char)c; // Operasi pergeseran bit bitwise ditambahkan nilai ASCII karakter
        return static_cast<int>(h % tableSize); // Mengembalikan nilai hasil konversi indeks batas aman tabel hash
    }

public:
    // Constructor: Menyiapkan 8 baris bucket kosong siap pakai untuk penempatan data voucher
    VoucherHashTable()
        : buckets(DEFAULT_SIZE, nullptr), tableSize(DEFAULT_SIZE), itemCount(0) {}

    // Destructor: Menjamin pembersihan memori objek voucher saat siklus hidup instance berakhir
    ~VoucherHashTable() { clear(); }

    // Fungsi untuk mendaftarkan data voucher belanja baru atau meng-update properti voucher lama
    void insert(const std::string& kode, int persen,
                const std::string& deskripsi, bool aktif = true) {
        int idx = hashFn(kode); // Mencari posisi indeks penampung lewat kalkulasi teks djb2 hash
        // Menelusuri linked list internal di bucket tersebut guna memeriksa duplikasi kode
        for (VoucherEntry* c = buckets[idx]; c; c = c->next) {
            if (c->kode == kode) { // Jika mendapati kode voucher yang diinput sudah ada sebelumnya
                c->persen = persen; c->deskripsi = deskripsi; c->aktif = aktif; // Overwrite data lama dengan yang baru
                return; // Proses pembaruan selesai, lewati pembuatan node baru
            }
        }
        // Jika benar-benar baru, buat instansiasi objek VoucherEntry di memori heap
        VoucherEntry* e = new VoucherEntry(kode, persen, deskripsi, aktif);
        e->next       = buckets[idx]; // Menautkan sisa rantai linked list lama ke belakang node baru
        buckets[idx]  = e;            // Memosisikan node baru ini di baris terdepan bucket (head)
        itemCount++; // Menaikkan indikasi jumlah total koleksi voucher
    }

    // Fungsi pencarian informasi voucher berdasarkan kode unik teks secara konstan O(1) rata-rata
    const VoucherEntry* lookup(const std::string& kode) const {
        int idx = hashFn(kode); // Mencari tahu di indeks ke berapa file voucher disimpan
        // Menelusuri sepanjang linked list pengaman pada posisi indeks bucket tersebut
        for (VoucherEntry* c = buckets[idx]; c; c = c->next)
            // Memastikan kode teks cocok persis dan status operasional voucher tersebut masih dinyatakan aktif
            if (c->kode == kode && c->aktif) return c; 
        return nullptr; // Mengembalikan status kosong jika voucher kadaluwarsa/tidak terdaftar
    }

    // Fungsi pembatalan fungsi voucher tanpa menghapus node dari memori (Soft Delete) demi integritas histori data
    void deactivate(const std::string& kode) {
        int idx = hashFn(kode); // Menemukan letak bucket target pembatalan voucher
        // Menelusuri elemen rantai pada indeks bersangkutan
        for (VoucherEntry* c = buckets[idx]; c; c = c->next)
            if (c->kode == kode) { c->aktif = false; return; } // Mematikan flag status ke posisi false dan keluar
    }

    // Fungsi total pembersihan alokasi heap memori untuk seluruh komponen terdaftar dalam tabel voucher
    void clear() {
        for (int i = 0; i < tableSize; ++i) { // Membuka slot array bucket baris demi baris
            VoucherEntry* c = buckets[i]; // Memposisikan di kepala simpul linked list ke-i
            while (c) { VoucherEntry* n = c->next; delete c; c = n; } // Loop menghapus node secara berantai sampai habis
            buckets[i] = nullptr; // Mereset pointer penunjuk bucket ke status kosong kembali
        }
        itemCount = 0; // Mengeset ulang jumlah item terhitung menjadi nol kembali
    }

    // Fungsi konstan pengambil total item data
    int count()   const { return itemCount; }

    // Fungsi penyusun rekaman laporan performa pemetaan data voucher (mendeteksi tingkat kolisi string)
    std::string getStatsString() const {
        int used = 0, collisions = 0; // Inisialisasi variabel bantu analisis data statistik
        for (int i = 0; i < tableSize; ++i) { // Iterasi meninjau semua slot ruang bucket voucher
            if (!buckets[i]) continue; used++; // Lewati jika kosong, catat slot terpakai jika berisi data
            int chain = 0; // Hitung kedalaman tumpukan mata rantai terisi
            for (VoucherEntry* c = buckets[i]; c; c = c->next) chain++; 
            if (chain > 1) collisions += chain - 1; // Mendeteksi kasus tabrakan hash string kode voucher
        }
        // Menyusun strings teks visual untuk disajikan pada halaman informasi pengawasan dasbor admin
        return "=== VOUCHER HASH TABLE STATS ===\n"
               "Table Size  : " + std::to_string(tableSize) + " bucket\n"
               "Vouchers    : " + std::to_string(itemCount) + "\n"
               "Buckets Used: " + std::to_string(used) + "\n"
               "Collisions  : " + std::to_string(collisions) + "\n"
               " [djb2 hash — O(1) avg lookup per kode voucher]";
    }
};


// =========================================================
// 3. ORDER HASH TABLE — Key: string (Order/Resi ID)
// =========================================================

// Struktur data utama yang mewakili log informasi pelacakan paket pesanan barang (Resi kurir)
struct OrderHashEntry {
    std::string orderId;         // Kunci pelacakan utama (Key) berformat teks (Contoh: "PKT-10023")
    std::string namaPelanggan;   // Nama dari pembeli pemesan barang
    std::string kotaAsal;        // Kota asal pengiriman barang gudang supplier
    std::string kotaTujuan;      // Kota alamat tujuan pengiriman rumah pemesan
    std::string statusLogistik;  // Status pengiriman logistik terkini (Misal: "DIKIRIM", "TRANSIT")
    std::string lokasiSekarang;  // Titik koordinat lokasi atau nama kota posisi fisik paket saat ini berada
    int         sisaWaktuJam;    // Estimasi sisa durasi pengantaran paket dalam satuan hitungan jam
    bool        isInstant;       // Penanda opsi kurir (true = Paket Kilat Instant, false = Reguler)
    std::string lastUpdate;      // Catatan waktu tanggal dan jam terakhir kali posisi paket diperbarui oleh sistem
    OrderHashEntry* next = nullptr; // Pointer penyambung rantai data logistik berikutnya apabila terpetakan di indeks sama

    // Constructor komplit pembentuk cetakan objek pelacakan pesanan baru
    OrderHashEntry(const std::string& oid, const std::string& nama,
                   const std::string& asal, const std::string& tujuan,
                   const std::string& status, const std::string& lokasi,
                   int jam, bool instant, const std::string& update)
        : orderId(oid), namaPelanggan(nama), kotaAsal(asal), kotaTujuan(tujuan),
          statusLogistik(status), lokasiSekarang(lokasi),
          sisaWaktuJam(jam), isInstant(instant), lastUpdate(update), next(nullptr) {}
};

// Kelas penangan struktur data hash map khusus pencarian dan pembaruan resi paket orderan pelanggan
class OrderHashTable {
private:
    static const int DEFAULT_SIZE = 16;  // Mengatur slot dasar bucket pelacakan sebanyak 16 entri direktori
    std::vector<OrderHashEntry*> buckets; // Dynamic array berisi pointer penunjuk mata rantai log logistik
    int tableSize;                       // Menyimpan total jangkauan indeks slot pelacak resi saat ini
    int itemCount;                       // Menyimpan jumlah total paket resi aktif yang beredar di sistem

    // Mengimplementasikan algoritma djb2 hashing untuk menghasilkan konversi indeks dari pola string teks "PKT-XXXXX"
    int hashFn(const std::string& key) const {
        unsigned long h = 5381; // Konstanta utama generator keacakan algoritma djb2
        for (char c : key) h = ((h << 5) + h) + (unsigned char)c; // Rumus iterasi multiplikasi bit geser karakter teks resi
        return static_cast<int>(h % tableSize); // Mengembalikan nilai sisa hasil bagi sebagai nomor indeks bucket resmi
    }

public:
    // Constructor: Melakukan persiapan memori tabel pelacakan resi berukuran 16 slot diisi penunjuk kosong
    OrderHashTable()
        : buckets(DEFAULT_SIZE, nullptr), tableSize(DEFAULT_SIZE), itemCount(0) {}

    // Destructor: Otomatis melenyapkan tumpukan node order logistik secara berantai guna menghindari memory leaks
    ~OrderHashTable() { clear(); }

    // Fungsi Upsert (Insert atau Update): Otomatis mendaftarkan resi baru, atau mengubah status posisi paket jika resi sudah ada
    void upsert(const std::string& orderId, const std::string& namaPelanggan,
                const std::string& asal, const std::string& tujuan,
                const std::string& status, const std::string& lokasi,
                int jam, bool instant, const std::string& lastUpdate) {
        int idx = hashFn(orderId); // Mengalkulasi letak posisi slot indeks bucket pelacakan resi dari fungsi djb2
        // Melakukan penelusuran linked list di area bucket indeks terpilih untuk mencari keberadaan ID resi tersebut
        for (OrderHashEntry* c = buckets[idx]; c; c = c->next) {
            if (c->orderId == orderId) { // Apabila nomor resi order didapati sudah terdaftar di database logistik
                c->statusLogistik = status; // Melakukan pembaruan (update) status kirim paket logistik terkini
                c->lokasiSekarang = lokasi; // Memperbarui kota titik singgah paket terbaru
                c->sisaWaktuJam   = jam;    // Meng-update sisa waktu estimasi tiba di tujuan
                c->lastUpdate     = lastUpdate; // Memperbarui tanda waktu riwayat pelacakan sistem kurir
                return; // Proses pembaruan informasi log resi selesai, hentikan laju fungsi
            }
        }
        // Bila nomor resi tidak ditemukan, buat record entri data paket logistik baru di memori heap
        OrderHashEntry* e = new OrderHashEntry(
            orderId, namaPelanggan, asal, tujuan, status, lokasi, jam, instant, lastUpdate);
        e->next      = buckets[idx]; // Menghubungkan ekor simpul baru ke sisa rantai linked list yang telah ada sebelumnya
        buckets[idx] = e;            // Mengangkat simpul anyar ini sebagai posisi kepala rantai terdepan pada bucket
        itemCount++; // Mengakumulasikan total kuantitas paket logistik terdata
    }

    // Fungsi pencarian cepat riwayat logistik pengantaran paket pesanan berdasarkan kode Resi / Order ID
    const OrderHashEntry* lookup(const std::string& orderId) const {
        int idx = hashFn(orderId); // Mengonversi teks string resi ke bentuk nomor indeks array bucket logistik
        // Berjalan menelusuri rantai simpul berurutan di dalam bucket indeks terkait
        for (OrderHashEntry* c = buckets[idx]; c; c = c->next)
            if (c->orderId == orderId) return c; // Mengembalikan pointer objek jika nomor resi terbukti valid dan cocok
        return nullptr; // Mengembalikan nilai kosong apabila resi salah ketik atau data belum diinput kurir
    }

    // Fungsi pengosongan total isi tabel hash logistik paket resi belanja
    void clear() {
        for (int i = 0; i < tableSize; ++i) { // Membuka map jalur slot array baris per baris
            OrderHashEntry* c = buckets[i]; // Memposisikan di titik hulu kepala linked list slot ke-i
            while (c) { OrderHashEntry* n = c->next; delete c; c = n; } // Eksekusi penghapusan node silih berganti sampai habis
            buckets[i] = nullptr; // Mengembalikan nilai aman pointer bucket ke kondisi kosong (nullptr)
        }
        itemCount = 0; // Menyetel total kuantitas monitor pelacakan kembali ke angka awal nol
    }

    // Fungsi konstan pendeteksi total kuantitas record paket order terkumpul saat ini
    int count() const { return itemCount; }

    // Fungsi penyusun resume cetak informasi kestabilan distribusi data hash map manajemen order resi logistik
    std::string getStatsString() const {
        int used = 0, maxChain = 0, collisions = 0; // Variabel kalkulator internal kondisi efisiensi memori tabel hash
        for (int i = 0; i < tableSize; ++i) { // Melakukan iterasi berputar memeriksa seluruh slot bucket logistik order
            if (!buckets[i]) continue; used++; // Jika slot kosong dilewati, jika terisi naikkan angka total slot aktif
            int chain = 0; // Mengukur seberapa panjang tumpukan data yang menempati indeks bucket yang sama
            for (OrderHashEntry* c = buckets[i]; c; c = c->next) chain++; // Menghitung kedalaman rantai
            if (chain > 1) collisions += chain - 1; // Menghitung akumulasi total kasus tabrakan indeks resi yang terjadi
            if (chain > maxChain) maxChain = chain; // Mengunci data rekor ukuran rantai linked list terdalam di tabel order
        }
        // Memformat string utuh berisi evaluasi diagnostik kapasitas memori untuk antarmuka UI dasbor monitoring pengiriman
        return "=== ORDER HASH TABLE STATS ===\n"
               "Table Size    : " + std::to_string(tableSize) + " bucket\n"
               "Orders        : " + std::to_string(itemCount) + "\n"
               "Buckets Used  : " + std::to_string(used) + "\n"
               "Max Chain Len : " + std::to_string(maxChain) + "\n"
               "Collisions    : " + std::to_string(collisions) + "\n"
               " [djb2 hash — O(1) avg lookup per Order ID / Resi]";
    }
};

// Menutup pembungkusan mekanisme macro preprocessor guard HASH_TABLE_H di awal berkas program
#endif // HASH_TABLE_H