#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <string>
#include <vector>
#include <algorithm>

// =========================================================
// AVL TREE — Penyimpanan Data Produk Utama (by ID)
//
// AVL Tree = BST yang selalu menjaga keseimbangan otomatis
// melalui operasi rotasi setiap kali insert/delete.
//
// Invariant AVL: |height(kiri) - height(kanan)| ≤ 1
//
// Operasi:
//   insert   → O(log N) dijamin (karena selalu balanced)
//   search   → O(log N) dijamin
//   remove   → O(log N) dijamin
//   inOrder  → O(N) untuk listing semua produk
// =========================================================

struct AVLNode {
    int         id;
    std::string name;
    std::string category;
    long        price;
    std::string origin;
    int         height;  // tinggi node untuk hitung balance factor
    AVLNode*    left;
    AVLNode*    right;

    AVLNode(int i, std::string n, std::string c, long p, std::string o)
        : id(i), name(n), category(c), price(p), origin(o),
        height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    AVLNode* root;

    // ---- Helper dasar ----

    int getHeight(AVLNode* n) const {
        return n ? n->height : 0;
    }

    int getBalanceFactor(AVLNode* n) const {
        return n ? getHeight(n->left) - getHeight(n->right) : 0;
    }

    void updateHeight(AVLNode* n) {
        if (n) n->height = 1 + std::max(getHeight(n->left), getHeight(n->right));
    }

    // ---- Rotasi ----

    // Rotasi kanan (untuk kasus Left-Left)
    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x  = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left  = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    // Rotasi kiri (untuk kasus Right-Right)
    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y  = x->right;
        AVLNode* T2 = y->left;
        y->left  = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    // Rebalance setelah insert/delete
    AVLNode* rebalance(AVLNode* node) {
        updateHeight(node);
        int bf = getBalanceFactor(node);

        // Left-Left: rotasi kanan
        if (bf > 1 && getBalanceFactor(node->left) >= 0)
            return rotateRight(node);

        // Left-Right: rotasi kiri dulu, lalu kanan
        if (bf > 1 && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right-Right: rotasi kiri
        if (bf < -1 && getBalanceFactor(node->right) <= 0)
            return rotateLeft(node);

        // Right-Left: rotasi kanan dulu, lalu kiri
        if (bf < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node; // sudah seimbang
    }

    // ---- Insert rekursif ----
    AVLNode* insertRek(AVLNode* node, int id, const std::string& name,
                       const std::string& cat, long price, const std::string& origin) {
        if (node == nullptr)
            return new AVLNode(id, name, cat, price, origin);

        if (id < node->id)
            node->left  = insertRek(node->left,  id, name, cat, price, origin);
        else if (id > node->id)
            node->right = insertRek(node->right, id, name, cat, price, origin);
        else {
            // ID sudah ada → update data produk
            node->name     = name;
            node->category = cat;
            node->price    = price;
            node->origin   = origin;
            return node;
        }

        return rebalance(node);
    }

    // ---- Cari node minimum (untuk delete) ----
    AVLNode* findMin(AVLNode* node) const {
        while (node->left) node = node->left;
        return node;
    }

    // ---- Delete rekursif ----
    AVLNode* deleteRek(AVLNode* node, int id) {
        if (node == nullptr) return nullptr;

        if (id < node->id) {
            node->left  = deleteRek(node->left,  id);
        } else if (id > node->id) {
            node->right = deleteRek(node->right, id);
        } else {
            // Node ditemukan
            if (!node->left || !node->right) {
                AVLNode* child = node->left ? node->left : node->right;
                delete node;
                return child; // bisa nullptr (leaf) atau satu anak
            }
            // Dua anak: ganti dengan in-order successor (min di subtree kanan)
            AVLNode* successor = findMin(node->right);
            node->id       = successor->id;
            node->name     = successor->name;
            node->category = successor->category;
            node->price    = successor->price;
            node->origin   = successor->origin;
            node->right    = deleteRek(node->right, successor->id);
        }

        return rebalance(node);
    }

    // ---- Search rekursif ----
    AVLNode* searchRek(AVLNode* node, int id) const {
        if (node == nullptr || node->id == id) return node;
        if (id < node->id) return searchRek(node->left,  id);
        else                return searchRek(node->right, id);
    }

    // ---- In-Order Traversal (ascending by ID) ----
    void inOrderRek(AVLNode* node, std::vector<AVLNode*>& result) const {
        if (!node) return;
        inOrderRek(node->left, result);
        result.push_back(node);
        inOrderRek(node->right, result);
    }

    // ---- Hapus semua node ----
    void clearRek(AVLNode* node) {
        if (!node) return;
        clearRek(node->left);
        clearRek(node->right);
        delete node;
    }

    // ---- Visualisasi pohon untuk UI edukasi ----
    void visualizeRek(AVLNode* node, std::string prefix,
                      bool isLeft, std::string& out) const {
        if (!node) return;
        out += prefix + (isLeft ? "├── " : "└── ");
        int bf = getBalanceFactor(node);
        out += "ID:" + std::to_string(node->id) +
               " [bf=" + std::to_string(bf) + "] " +
               node->name + "\n";
        visualizeRek(node->left,  prefix + (isLeft ? "│   " : "    "), true,  out);
        visualizeRek(node->right, prefix + (isLeft ? "│   " : "    "), false, out);
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() { clearRek(root); }

    // Masukkan atau update produk
    void insert(int id, const std::string& name, const std::string& cat,
                long price, const std::string& origin) {
        root = insertRek(root, id, name, cat, price, origin);
    }

    // Hapus produk berdasarkan ID
    void remove(int id) {
        root = deleteRek(root, id);
    }

    // Cari produk berdasarkan ID — O(log N) dijamin
    AVLNode* search(int id) const {
        return searchRek(root, id);
    }

    // Kembalikan semua produk urut ID ascending
    std::vector<AVLNode*> getAllSortedById() const {
        std::vector<AVLNode*> result;
        inOrderRek(root, result);
        return result;
    }

    bool isEmpty() const { return root == nullptr; }

    int getTreeHeight() const { return getHeight(root); }

    void clear() {
        clearRek(root);
        root = nullptr;
    }

    // Kembalikan string visualisasi pohon AVL
    std::string getTreeVisualization() const {
        if (!root) return "(AVL Tree kosong)";
        std::string out = "=== AVL TREE (by ID Produk) — Height: " +
                          std::to_string(getTreeHeight()) + " ===\n";
        out += "Root: ID " + std::to_string(root->id) +
               " [bf=" + std::to_string(getBalanceFactor(root)) + "] " +
               root->name + "\n";
        visualizeRek(root->left,  "", true,  out);
        visualizeRek(root->right, "", false, out);
        out += "\n[bf = balance factor; harus -1, 0, atau +1 pada setiap node]";
        return out;
    }

    // Cek apakah tree valid AVL (semua balance factor ∈ {-1, 0, 1})
    bool isValidAVL(AVLNode* node = nullptr, bool useRoot = true) const {
        if (useRoot) node = root;
        if (!node) return true;
        int bf = getBalanceFactor(node);
        if (bf < -1 || bf > 1) return false;
        return isValidAVL(node->left, false) && isValidAVL(node->right, false);
    }
};

#endif // AVL_TREE_H