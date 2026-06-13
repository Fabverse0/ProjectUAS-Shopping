# 🛒 CSHOP — Sistem Belanja & Logistik Berbasis Struktur Data

CSHOP adalah aplikasi desktop simulasi e-commerce dan sistem logistik yang dibangun menggunakan **C++17** dan **Qt Framework**. Proyek ini dirancang sebagai implementasi nyata dari berbagai struktur data dan algoritma klasik dalam satu aplikasi yang terintegrasi penuh.

---

## 📋 Daftar Isi

- [Fitur Utama](#fitur-utama)
- [Struktur Data yang Digunakan](#struktur-data-yang-digunakan)
- [Persyaratan Sistem](#persyaratan-sistem)
- [Cara Build & Menjalankan](#cara-build--menjalankan)
- [Akun yang Tersedia](#akun-yang-tersedia)
- [Alur Penggunaan](#alur-penggunaan)
- [Kode Promo](#kode-promo)
- [Jaringan Hub Logistik](#jaringan-hub-logistik)
- [Struktur File](#struktur-file)

---

## Fitur Utama

**Untuk Pelanggan:**
- Browsing katalog produk per kategori (Elektronik, Pakaian, Perabotan) dengan navigasi maju/mundur
- Pencarian produk global via Binary Search (by ID) dan prefix search (by nama)
- Pengurutan produk berdasarkan harga menggunakan BST In-Order / Reverse In-Order Traversal
- Keranjang belanja dengan sistem split-order otomatis per gudang asal
- Pemilihan layanan pengiriman **per sub-paket** secara independen (Instan / Reguler / Kargo Jauh)
- Penerapan kode promo/diskon secara real-time
- Lacak status paket secara mandiri menggunakan Resi ID

**Untuk Admin:**
- Dashboard manajemen status pengiriman seluruh paket aktif
- Update lokasi terkini paket dengan perhitungan sisa waktu otomatis
- Proses antrean paket siap kirim dengan sistem prioritas (Instan didahulukan)
- Kelola produk: tambah, edit, hapus (tersinkron ke AVL Tree, BST, Hash Table, dan Binary Search Index)
- Visualisasi struktur AVL Tree dan BST secara real-time
- Simulasi BFS dan DFS pada jaringan hub logistik
- Quick Lookup hash table untuk produk, voucher, dan order

---

## Struktur Data yang Digunakan

Setiap struktur data dipilih secara spesifik sesuai kebutuhan fitur:

### 1. Double Linked List — Katalog Produk (`catalog_list.h`)
Menyimpan daftar produk per kategori. Navigasi dua arah (Previous/Next) memanfaatkan pointer `prev` dan `next`. Mendukung manajemen stok, pencarian substring, dan sorting menggunakan `std::sort` O(N log N).

### 2. Single Linked List — Keranjang Belanja (`cart_list.h`)
Menyimpan item yang dimasukkan pelanggan. Insert di akhir, hapus by index. Mendukung ekstraksi data asal gudang untuk fitur split-order saat checkout.

### 3. Circular Linked List — Banner Promo
Menampilkan rotasi kode promo di halaman katalog secara otomatis setiap 3 detik. Setelah node terakhir, pointer kembali ke node pertama secara siklik.

### 4. Stack (Linked List) — Riwayat Navigasi (`nav_stack.h`)
Mencatat riwayat halaman yang dikunjungi. Tombol "Back" melakukan `pop()` untuk kembali ke halaman sebelumnya. Diimplementasikan dengan linked list (bukan array) sehingga ukuran dinamis.

### 5. Queue (Priority Queue) — Antrean Pesanan (`order_queue.h`)
Mengantrekan sub-paket yang siap dikirim. Paket **Instan** selalu disisipkan di depan paket Reguler (priority enqueue). Admin memproses satu per satu dari depan antrian (dequeue FIFO).

### 6. Binary Search — Pencarian Produk (`binary_search.h`)
Membangun indeks terurut dari seluruh produk. Pencarian by ID menggunakan binary search eksak O(log N). Pencarian by nama menggunakan prefix lower-bound search O(log N + k). Dilengkapi log langkah-langkah pencarian untuk keperluan edukasi.

### 7. BST (Binary Search Tree) — Urut Harga (`bst.h`)
Produk diinsert ke BST berdasarkan harga. In-Order Traversal menghasilkan urutan harga ascending, Reverse In-Order menghasilkan descending — tanpa perlu sorting ulang. Struktur pohon dapat divisualisasikan dari dalam aplikasi.

### 8. AVL Tree — Penyimpanan Produk Utama (`avl_tree.h`)
Seluruh produk disimpan di AVL Tree dengan key berupa ID produk. Auto-rebalance setiap insert/delete menjamin height O(log N), sehingga search, insert, dan delete selalu O(log N). Visualisasi balance factor tiap node tersedia di panel Admin.

### 9. Hash Table — Quick Lookup O(1) (`hash_table.h`)
Tiga tabel hash terpisah:
- **ProductHashTable** — lookup produk by ID (Fibonacci hashing, int key)
- **VoucherHashTable** — lookup kode voucher/diskon (djb2 hash, string key)
- **OrderHashTable** — lookup resi/order by ID (djb2 hash, string key)

Semua menggunakan Separate Chaining untuk collision handling dan rehash otomatis saat load factor > 0.75.

### 10. Graph + BFS + DFS — Jaringan Logistik (`graph.h`)
Jaringan hub logistik dimodelkan sebagai directed weighted graph (adjacency list). BFS (Queue-based) menemukan rute dengan hop minimum. DFS (Stack eksplisit, bukan rekursi) mengaudit konektivitas seluruh hub dari titik awal tertentu.

---

## Persyaratan Sistem

| Komponen | Minimum |
|---|---|
| Compiler | GCC 9+ / Clang 10+ / MSVC 2019+ (C++17) |
| Qt | Qt 5.15+ atau Qt 6.x |
| CMake | 3.16+ |
| OS | Windows 10+, Linux (Ubuntu 20.04+), macOS 11+ |

---

## Cara Build & Menjalankan

**1. Clone / salin seluruh file proyek ke satu folder.**

**2. Build menggunakan Qt Creator (cara termudah):**
```
Buka Qt Creator → Open Project → pilih CMakeLists.txt → klik Run
```

**3. Build via command line (CMake):**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
./CSHOP          # Linux/macOS
CSHOP.exe        # Windows
```

**4. Build via qmake (alternatif):**
```bash
qmake CSHOP.pro
make -j4
```

Tidak ada dependensi eksternal selain Qt Widgets. Seluruh struktur data diimplementasikan dari nol dalam file header masing-masing.

---

## Akun yang Tersedia

| Username | Password | Role |
|---|---|---|
| `pelanggan1` | `pass123` | Pelanggan |
| `pelanggan2` | `pass456` | Pelanggan |
| `admin` | `admin123` | Admin |

Pelanggan hanya dapat mengakses Katalog, Keranjang, dan Lacak Paket. Admin mendapat akses penuh termasuk Antrean, Jaringan Rute, Dashboard, Kelola Produk, dan Quick Lookup Hash Table.

---

## Alur Penggunaan

### Sebagai Pelanggan

```
Login → Katalog Produk
      → Pilih kategori (Elektronik / Pakaian / Perabotan)
      → Navigasi produk (Previous / Next) atau cari via 🔍 Cari Produk Global
      → Urutkan harga via 📊 Urut Harga (BST)
      → Tambah ke Keranjang

Keranjang Belanja
      → Pilih alamat tujuan (Hub Jakarta / Sub-Hub Bekasi / Hub Surabaya / Hub Merauke)
      → Klik ⚙ Atur Layanan Per Paket → pilih Instan atau Reguler untuk tiap sub-paket
      → Masukkan kode promo (opsional)
      → Klik 🚀 Checkout

Lacak Paket Saya
      → Masukkan Resi ID (format PKT-XXXXX)
      → Lihat status, lokasi terkini, dan estimasi waktu tiba
```

### Sebagai Admin

```
Login → Dashboard Admin
      → Pilih paket dari daftar → pilih lokasi terkini → Update Status

Antrean Pesanan
      → Lihat semua paket dalam antrean (Instan tampil di atas)
      → Klik Proses & Serahkan ke Kurir (Dequeue)

Kelola Produk
      → Tambah / Edit / Hapus produk (tersinkron ke semua struktur data)
      → Cari produk via AVL Tree Search

Jaringan Rute
      → Lihat adjacency list seluruh hub
      → Jalankan BFS (rute minimum hop) atau DFS (audit konektivitas)

Quick Lookup (Hash)
      → Cari produk by ID, voucher by kode, atau order by Resi ID — O(1)
```

---

## Kode Promo

| Kode | Diskon | Keterangan |
|---|---|---|
| `WELCOME10` | 10% | Diskon untuk pelanggan baru |
| `HEMAT20` | 20% | Hemat 20% semua produk |
| `FLASH30` | 30% | Flash sale |
| `CCART50` | 50% | Diskon besar semua produk |
| `GRATIS` | 100% | Produk + ongkir gratis |

---

## Jaringan Hub Logistik

Topologi jaringan hub yang digunakan dalam simulasi:

```
Sub-Hub Bekasi ──(2 jam)──► Hub Jakarta ──(15 jam)──► Hub Surabaya ──(72 jam)──► Hub Merauke
```

Estimasi waktu tempuh antar hub (simetris dua arah):

| Dari | Ke | Estimasi |
|---|---|---|
| Sub-Hub Bekasi | Hub Jakarta | 2 jam |
| Hub Jakarta | Hub Surabaya | 15 jam |
| Hub Surabaya | Hub Merauke | 72 jam |
| Hub Jakarta | Hub Merauke | 87 jam |
| Sub-Hub Bekasi | Hub Surabaya | 17 jam |
| Sub-Hub Bekasi | Hub Merauke | 89 jam |

Rute dengan estimasi ≤ 24 jam mendukung layanan **Instan** (+Rp 25.000) dan **Reguler** (+Rp 10.000). Rute > 24 jam otomatis menggunakan **Kargo Jauh** (+Rp 40.000) tanpa opsi instan.

---

## Struktur File

```
CSHOP/
├── main.cpp              — Entry point aplikasi Qt
├── mainwindow.h          — Deklarasi MainWindow, semua widget & member
├── mainwindow.cpp        — Implementasi seluruh logika UI dan bisnis
├── mainwindow.ui         — File UI Qt Designer
├── CMakeLists.txt        — Konfigurasi build CMake
│
├── catalog_list.h        — Double Linked List (katalog produk)
├── cart_list.h           — Single Linked List (keranjang belanja)
├── nav_stack.h           — Stack Linked List (riwayat navigasi)
├── order_queue.h         — Priority Queue (antrean pesanan)
├── binary_search.h       — Binary Search (pencarian produk)
├── bst.h                 — Binary Search Tree (urut harga)
├── avl_tree.h            — AVL Tree (penyimpanan produk utama)
├── hash_table.h          — Hash Table x3 (produk, voucher, order)
└── graph.h               — Graph + BFS + DFS (jaringan logistik)
```

---

> Proyek ini dibuat sebagai implementasi praktis mata kuliah Struktur Data, mendemonstrasikan bagaimana setiap struktur data memiliki kasus penggunaan yang paling tepat dalam sistem nyata.
