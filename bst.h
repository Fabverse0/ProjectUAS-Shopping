#ifndef BST_H
#define BST_H

#include <string>
#include <vector>

// =========================================================
// BINARY SEARCH TREE (BST) — Tampilan Katalog Urut Harga
//
// Properti BST:
//   - Node kiri  : harga < harga node saat ini
//   - Node kanan : harga >= harga node saat ini
//
// Operasi:
//   insert       → O(log N) rata-rata, O(N) worst case
//   inOrder      → O(N) menghasilkan urutan harga ascending
//   reverseOrder → O(N) menghasilkan urutan harga descending
// =========================================================

struct BSTNode {
    int         id;
    std::string name;
    std::string category;
    long        price;
    std::string origin;
    BSTNode*    left;
    BSTNode*    right;

    BSTNode(int i, std::string n, std::string c, long p, std::string o)
        : id(i), name(n), category(c), price(p), origin(o),
        left(nullptr), right(nullptr) {}
};

class PriceBST {
private:
    BSTNode* root;

    // Rekursif insert
    BSTNode* insertRek(BSTNode* node, int id, const std::string& name,
                       const std::string& cat, long price, const std::string& origin) {
        if (node == nullptr) {
            return new BSTNode(id, name, cat, price, origin);
        }
        if (price < node->price) {
            node->left  = insertRek(node->left,  id, name, cat, price, origin);
        } else {
            node->right = insertRek(node->right, id, name, cat, price, origin);
        }
        return node;
    }

    // In-Order: kiri → root → kanan (ascending harga)
    void inOrderRek(BSTNode* node, std::vector<BSTNode*>& result) const {
        if (node == nullptr) return;
        inOrderRek(node->left, result);
        result.push_back(node);
        inOrderRek(node->right, result);
    }

    // Reverse In-Order: kanan → root → kiri (descending harga)
    void reverseOrderRek(BSTNode* node, std::vector<BSTNode*>& result) const {
        if (node == nullptr) return;
        reverseOrderRek(node->right, result);
        result.push_back(node);
        reverseOrderRek(node->left, result);
    }

    // Hapus seluruh tree (destructor helper)
    void clearRek(BSTNode* node) {
        if (node == nullptr) return;
        clearRek(node->left);
        clearRek(node->right);
        delete node;
    }

    // Visualisasi struktur BST (untuk panel edukasi)
    void printTreeRek(BSTNode* node, std::string prefix,
                      bool isLeft, std::string& out) const {
        if (node == nullptr) return;
        out += prefix + (isLeft ? "├── " : "└── ");
        out += "[" + std::to_string(node->price) + "] " + node->name + "\n";
        printTreeRek(node->left,  prefix + (isLeft ? "│   " : "    "), true,  out);
        printTreeRek(node->right, prefix + (isLeft ? "│   " : "    "), false, out);
    }

public:
    PriceBST() : root(nullptr) {}

    ~PriceBST() {
        clearRek(root);
    }

    // Masukkan produk ke BST berdasarkan harga
    void insert(int id, const std::string& name, const std::string& cat,
                long price, const std::string& origin) {
        root = insertRek(root, id, name, cat, price, origin);
    }

    // Kembalikan list produk terurut ascending (In-Order Traversal)
    std::vector<BSTNode*> getSortedAscending() const {
        std::vector<BSTNode*> result;
        inOrderRek(root, result);
        return result;
    }

    // Kembalikan list produk terurut descending (Reverse In-Order)
    std::vector<BSTNode*> getSortedDescending() const {
        std::vector<BSTNode*> result;
        reverseOrderRek(root, result);
        return result;
    }

    // Cek apakah BST kosong
    bool isEmpty() const { return root == nullptr; }

    // Hapus semua node dan reset
    void clear() {
        clearRek(root);
        root = nullptr;
    }

    // Kembalikan string visualisasi struktur BST (untuk UI edukasi)
    std::string getTreeVisualization() const {
        if (root == nullptr) return "(BST kosong)";
        std::string out = "=== STRUKTUR BST (berdasarkan Harga) ===\n";
        // Root node
        out += "Root: [" + std::to_string(root->price) + "] " + root->name + "\n";
        printTreeRek(root->left,  "", true,  out);
        printTreeRek(root->right, "", false, out);
        out += "\n[Kiri = harga lebih kecil | Kanan = harga lebih besar]";
        return out;
    }
};

#endif // BST_H