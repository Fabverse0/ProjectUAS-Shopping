#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <functional>

// =========================================================
// HASH TABLE — Quick Lookup O(1) Average
//
// Tiga tabel terpisah sesuai kebutuhan aplikasi CSHOP:
//
//   1. ProductHashTable   — lookup produk by ID (int key)
//   2. VoucherHashTable   — lookup voucher/diskon by kode (string key)
//   3. OrderHashTable     — lookup order by Order ID (string key)
//
// Implementasi: Separate Chaining (linked list per bucket)
// Hash function:
//   - int key  : hash(k) = k % tableSize
//   - str key  : polynomial rolling hash (djb2 variant)
//
// Load factor dijaga ≤ 0.75 (rehash otomatis)
// =========================================================


// =========================================================
// 1. PRODUCT HASH TABLE — Key: int (Product ID)
// =========================================================

struct ProductHashEntry {
    int         id;
    std::string name;
    std::string category;
    long        price;
    std::string origin;
    ProductHashEntry* next = nullptr;

    ProductHashEntry(int i, const std::string& n, const std::string& c,
                     long p, const std::string& o)
        : id(i), name(n), category(c), price(p), origin(o), next(nullptr) {}
};

class ProductHashTable {
private:
    static const int DEFAULT_SIZE = 16;
    std::vector<ProductHashEntry*> buckets;
    int tableSize;
    int itemCount;

    int hashFn(int key) const {
        // Fibonacci hashing untuk distribusi lebih merata
        return ((key * 2654435769U) >> (32 - __builtin_ctz(tableSize))) % tableSize;
    }

    void rehash() {
        int newSize = tableSize * 2;
        std::vector<ProductHashEntry*> newBuckets(newSize, nullptr);

        // Re-insert semua entry ke tabel baru
        for (int i = 0; i < tableSize; ++i) {
            ProductHashEntry* curr = buckets[i];
            while (curr) {
                ProductHashEntry* next = curr->next;
                int newIdx = ((curr->id * 2654435769U) >> (32 - __builtin_ctz(newSize))) % newSize;
                curr->next = newBuckets[newIdx];
                newBuckets[newIdx] = curr;
                curr = next;
            }
        }
        buckets   = newBuckets;
        tableSize = newSize;
    }

public:
    ProductHashTable()
        : buckets(DEFAULT_SIZE, nullptr), tableSize(DEFAULT_SIZE), itemCount(0) {}

    ~ProductHashTable() { clear(); }

    // Insert atau update produk — O(1) amortized
    void insert(int id, const std::string& name, const std::string& category,
                long price, const std::string& origin) {
        // Cek apakah ID sudah ada → update
        int idx = hashFn(id);
        ProductHashEntry* curr = buckets[idx];
        while (curr) {
            if (curr->id == id) {
                curr->name = name; curr->category = category;
                curr->price = price; curr->origin = origin;
                return;
            }
            curr = curr->next;
        }

        // Insert baru di kepala chain
        ProductHashEntry* newEntry = new ProductHashEntry(id, name, category, price, origin);
        newEntry->next = buckets[idx];
        buckets[idx]   = newEntry;
        itemCount++;

        // Rehash jika load factor > 0.75
        if (itemCount > tableSize * 3 / 4) rehash();
    }

    // Lookup by ID — O(1) average
    const ProductHashEntry* lookup(int id) const {
        int idx = hashFn(id);
        ProductHashEntry* curr = buckets[idx];
        while (curr) {
            if (curr->id == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // Hapus produk by ID
    void remove(int id) {
        int idx = hashFn(id);
        ProductHashEntry* curr = buckets[idx];
        ProductHashEntry* prev = nullptr;
        while (curr) {
            if (curr->id == id) {
                if (prev) prev->next = curr->next;
                else       buckets[idx] = curr->next;
                delete curr;
                itemCount--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    // Bersihkan seluruh tabel
    void clear() {
        for (int i = 0; i < tableSize; ++i) {
            ProductHashEntry* curr = buckets[i];
            while (curr) {
                ProductHashEntry* next = curr->next;
                delete curr;
                curr = next;
            }
            buckets[i] = nullptr;
        }
        itemCount = 0;
    }

    // Statistik untuk panel edukasi
    int  count()     const { return itemCount; }
    int  size()      const { return tableSize; }
    double loadFactor() const { return itemCount / (double)tableSize; }

    // Distribusi bucket (untuk visualisasi)
    std::string getStatsString() const {
        int used = 0, maxChain = 0, collisions = 0;
        for (int i = 0; i < tableSize; ++i) {
            if (!buckets[i]) continue;
            used++;
            int chain = 0;
            for (ProductHashEntry* c = buckets[i]; c; c = c->next) chain++;
            if (chain > 1) collisions += chain - 1;
            if (chain > maxChain) maxChain = chain;
        }
        return "=== PRODUCT HASH TABLE STATS ===\n"
               "Table Size    : " + std::to_string(tableSize) + " bucket\n"
                                             "Items         : " + std::to_string(itemCount) + "\n"
                                             "Load Factor   : " + std::to_string(loadFactor()).substr(0,4) + "\n"
                                                             "Buckets Used  : " + std::to_string(used) + "\n"
                                        "Max Chain Len : " + std::to_string(maxChain) + "\n"
                                            "Collisions    : " + std::to_string(collisions) + "\n"
                                              "[O(1) avg lookup — rehash otomatis jika LF > 0.75]";
    }
};


// =========================================================
// 2. VOUCHER HASH TABLE — Key: string (kode voucher)
// =========================================================

struct VoucherEntry {
    std::string kode;
    int         persen;
    std::string deskripsi;
    bool        aktif;          // bisa di-nonaktifkan oleh admin
    VoucherEntry* next = nullptr;

    VoucherEntry(const std::string& k, int p, const std::string& d, bool a = true)
        : kode(k), persen(p), deskripsi(d), aktif(a), next(nullptr) {}
};

class VoucherHashTable {
private:
    static const int DEFAULT_SIZE = 8;
    std::vector<VoucherEntry*> buckets;
    int tableSize;
    int itemCount;

    // djb2 hash — distribusi baik untuk string pendek
    int hashFn(const std::string& key) const {
        unsigned long h = 5381;
        for (char c : key) h = ((h << 5) + h) + (unsigned char)c;
        return static_cast<int>(h % tableSize);
    }

public:
    VoucherHashTable()
        : buckets(DEFAULT_SIZE, nullptr), tableSize(DEFAULT_SIZE), itemCount(0) {}

    ~VoucherHashTable() { clear(); }

    // Insert voucher baru
    void insert(const std::string& kode, int persen,
                const std::string& deskripsi, bool aktif = true) {
        int idx = hashFn(kode);
        // Update jika sudah ada
        for (VoucherEntry* c = buckets[idx]; c; c = c->next) {
            if (c->kode == kode) {
                c->persen = persen; c->deskripsi = deskripsi; c->aktif = aktif;
                return;
            }
        }
        VoucherEntry* e = new VoucherEntry(kode, persen, deskripsi, aktif);
        e->next       = buckets[idx];
        buckets[idx]  = e;
        itemCount++;
    }

    // Lookup by kode — O(1) average
    // Mengembalikan nullptr jika tidak ditemukan atau tidak aktif
    const VoucherEntry* lookup(const std::string& kode) const {
        int idx = hashFn(kode);
        for (VoucherEntry* c = buckets[idx]; c; c = c->next)
            if (c->kode == kode && c->aktif) return c;
        return nullptr;
    }

    // Nonaktifkan voucher (soft delete)
    void deactivate(const std::string& kode) {
        int idx = hashFn(kode);
        for (VoucherEntry* c = buckets[idx]; c; c = c->next)
            if (c->kode == kode) { c->aktif = false; return; }
    }

    void clear() {
        for (int i = 0; i < tableSize; ++i) {
            VoucherEntry* c = buckets[i];
            while (c) { VoucherEntry* n = c->next; delete c; c = n; }
            buckets[i] = nullptr;
        }
        itemCount = 0;
    }

    int count()   const { return itemCount; }

    std::string getStatsString() const {
        int used = 0, collisions = 0;
        for (int i = 0; i < tableSize; ++i) {
            if (!buckets[i]) continue; used++;
            int chain = 0;
            for (VoucherEntry* c = buckets[i]; c; c = c->next) chain++;
            if (chain > 1) collisions += chain - 1;
        }
        return "=== VOUCHER HASH TABLE STATS ===\n"
               "Table Size  : " + std::to_string(tableSize) + " bucket\n"
                                             "Vouchers    : " + std::to_string(itemCount) + "\n"
                                             "Buckets Used: " + std::to_string(used) + "\n"
                                        "Collisions  : " + std::to_string(collisions) + "\n"
                                              "[djb2 hash — O(1) avg lookup per kode voucher]";
    }
};


// =========================================================
// 3. ORDER HASH TABLE — Key: string (Order/Resi ID)
// =========================================================

struct OrderHashEntry {
    std::string orderId;
    std::string namaPelanggan;
    std::string kotaAsal;
    std::string kotaTujuan;
    std::string statusLogistik;
    std::string lokasiSekarang;
    int         sisaWaktuJam;
    bool        isInstant;
    std::string lastUpdate;
    OrderHashEntry* next = nullptr;

    OrderHashEntry(const std::string& oid, const std::string& nama,
                   const std::string& asal, const std::string& tujuan,
                   const std::string& status, const std::string& lokasi,
                   int jam, bool instant, const std::string& update)
        : orderId(oid), namaPelanggan(nama), kotaAsal(asal), kotaTujuan(tujuan),
        statusLogistik(status), lokasiSekarang(lokasi),
        sisaWaktuJam(jam), isInstant(instant), lastUpdate(update), next(nullptr) {}
};

class OrderHashTable {
private:
    static const int DEFAULT_SIZE = 16;
    std::vector<OrderHashEntry*> buckets;
    int tableSize;
    int itemCount;

    // djb2 hash untuk string order ID (format: "PKT-XXXXX")
    int hashFn(const std::string& key) const {
        unsigned long h = 5381;
        for (char c : key) h = ((h << 5) + h) + (unsigned char)c;
        return static_cast<int>(h % tableSize);
    }

public:
    OrderHashTable()
        : buckets(DEFAULT_SIZE, nullptr), tableSize(DEFAULT_SIZE), itemCount(0) {}

    ~OrderHashTable() { clear(); }

    // Insert atau update order — dipanggil saat checkout & admin update
    void upsert(const std::string& orderId, const std::string& namaPelanggan,
                const std::string& asal, const std::string& tujuan,
                const std::string& status, const std::string& lokasi,
                int jam, bool instant, const std::string& lastUpdate) {
        int idx = hashFn(orderId);
        // Update jika sudah ada
        for (OrderHashEntry* c = buckets[idx]; c; c = c->next) {
            if (c->orderId == orderId) {
                c->statusLogistik = status;
                c->lokasiSekarang = lokasi;
                c->sisaWaktuJam   = jam;
                c->lastUpdate     = lastUpdate;
                return;
            }
        }
        // Insert baru
        OrderHashEntry* e = new OrderHashEntry(
            orderId, namaPelanggan, asal, tujuan, status, lokasi, jam, instant, lastUpdate);
        e->next      = buckets[idx];
        buckets[idx] = e;
        itemCount++;
    }

    // Quick lookup by Order ID — O(1) average
    const OrderHashEntry* lookup(const std::string& orderId) const {
        int idx = hashFn(orderId);
        for (OrderHashEntry* c = buckets[idx]; c; c = c->next)
            if (c->orderId == orderId) return c;
        return nullptr;
    }

    void clear() {
        for (int i = 0; i < tableSize; ++i) {
            OrderHashEntry* c = buckets[i];
            while (c) { OrderHashEntry* n = c->next; delete c; c = n; }
            buckets[i] = nullptr;
        }
        itemCount = 0;
    }

    int count() const { return itemCount; }

    std::string getStatsString() const {
        int used = 0, maxChain = 0, collisions = 0;
        for (int i = 0; i < tableSize; ++i) {
            if (!buckets[i]) continue; used++;
            int chain = 0;
            for (OrderHashEntry* c = buckets[i]; c; c = c->next) chain++;
            if (chain > 1) collisions += chain - 1;
            if (chain > maxChain) maxChain = chain;
        }
        return "=== ORDER HASH TABLE STATS ===\n"
               "Table Size    : " + std::to_string(tableSize) + " bucket\n"
                                             "Orders        : " + std::to_string(itemCount) + "\n"
                                             "Buckets Used  : " + std::to_string(used) + "\n"
                                        "Max Chain Len : " + std::to_string(maxChain) + "\n"
                                            "Collisions    : " + std::to_string(collisions) + "\n"
                                              "[djb2 hash — O(1) avg lookup per Order ID / Resi]";
    }
};

#endif // HASH_TABLE_H