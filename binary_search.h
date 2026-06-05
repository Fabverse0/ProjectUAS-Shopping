#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

// =========================================================
// BINARY SEARCH — Pencarian Produk (by Nama / ID)
//
// Cara kerja:
//   1. Array produk diurutkan terlebih dahulu (O(N log N))
//   2. Binary Search dijalankan → O(log N) per query
//
// Dua mode pencarian:
//   - searchByName : mencocokkan nama produk (case-insensitive)
//   - searchById   : mencocokkan ID produk secara eksak
// =========================================================

struct ProductEntry {
    int         id;
    std::string name;
    std::string category;
    long        price;
    std::string origin;
};

class BinarySearch {
private:
    std::vector<ProductEntry> sortedByName; // diurutkan A-Z untuk search nama
    std::vector<ProductEntry> sortedById;   // diurutkan ascending untuk search ID
    bool isBuilt = false;

    // Ubah string ke huruf kecil (helper internal)
    static std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

public:
    // Tambahkan satu produk ke daftar (sebelum build index)
    void addProduct(int id, const std::string& name,
                    const std::string& category, long price,
                    const std::string& origin) {
        ProductEntry e{id, name, category, price, origin};
        sortedByName.push_back(e);
        sortedById.push_back(e);
        isBuilt = false; // tandai perlu rebuild
    }

    // Bangun indeks terurut (wajib dipanggil sebelum search)
    void buildIndex() {
        std::sort(sortedByName.begin(), sortedByName.end(),
                  [](const ProductEntry& a, const ProductEntry& b) {
                      return toLower(a.name) < toLower(b.name);
                  });
        std::sort(sortedById.begin(), sortedById.end(),
                  [](const ProductEntry& a, const ProductEntry& b) {
                      return a.id < b.id;
                  });
        isBuilt = true;
    }

    // -------------------------------------------------------
    // Binary Search by ID (exact match) → O(log N)
    // -------------------------------------------------------
    // Mengembalikan pointer ke entry, nullptr jika tidak ditemukan
    const ProductEntry* searchById(int targetId) {
        if (!isBuilt) buildIndex();

        int lo = 0, hi = static_cast<int>(sortedById.size()) - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (sortedById[mid].id == targetId) {
                return &sortedById[mid];
            } else if (sortedById[mid].id < targetId) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return nullptr; // tidak ditemukan
    }

    // -------------------------------------------------------
    // Binary Search by Name — menemukan semua produk yang
    // namanya diawali dengan keyword (prefix match, O(log N + k))
    // -------------------------------------------------------
    std::vector<ProductEntry> searchByNamePrefix(const std::string& keyword) {
        if (!isBuilt) buildIndex();

        std::string kw = toLower(keyword);
        std::vector<ProductEntry> result;

        // Cari batas bawah: item pertama yang >= keyword
        int lo = 0, hi = static_cast<int>(sortedByName.size()) - 1;
        int lowerBound = static_cast<int>(sortedByName.size()); // default: tidak ada
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            std::string midLow = toLower(sortedByName[mid].name);
            if (midLow >= kw) {
                lowerBound = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }

        // Kumpulkan semua entry yang dimulai dengan keyword
        for (int i = lowerBound; i < (int)sortedByName.size(); ++i) {
            std::string nameLow = toLower(sortedByName[i].name);
            if (nameLow.substr(0, kw.size()) == kw) {
                result.push_back(sortedByName[i]);
            } else {
                break; // sudah melewati prefix yang cocok
            }
        }
        return result;
    }

    // -------------------------------------------------------
    // Kembalikan ringkasan log proses Binary Search (by ID)
    // untuk keperluan tampilan edukasi di UI
    // -------------------------------------------------------
    std::string getSearchLog(int targetId) {
        if (!isBuilt) buildIndex();

        std::string log = "=== BINARY SEARCH LOG (by ID: " + std::to_string(targetId) + ") ===\n";
        int lo = 0, hi = static_cast<int>(sortedById.size()) - 1;
        int iterasi = 1;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            log += "Iterasi " + std::to_string(iterasi++) +
                   ": lo=" + std::to_string(lo) +
                   " mid=" + std::to_string(mid) +
                   " hi=" + std::to_string(hi) +
                   " → cek ID " + std::to_string(sortedById[mid].id);
            if (sortedById[mid].id == targetId) {
                log += " ✅ DITEMUKAN!\n";
                break;
            } else if (sortedById[mid].id < targetId) {
                log += " → geser lo ke kanan\n";
                lo = mid + 1;
            } else {
                log += " → geser hi ke kiri\n";
                hi = mid - 1;
            }
        }
        if (lo > hi) log += "❌ Produk tidak ditemukan.\n";
        log += "Total perbandingan: " + std::to_string(iterasi - 1) +
               " (dari " + std::to_string(sortedById.size()) + " produk)";
        return log;
    }

    // Reset semua data (misal saat reload katalog)
    void clear() {
        sortedByName.clear();
        sortedById.clear();
        isBuilt = false;
    }

    int count() const {
        return static_cast<int>(sortedById.size());
    }
};

#endif // BINARY_SEARCH_H