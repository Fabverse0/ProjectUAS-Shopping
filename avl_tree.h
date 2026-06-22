// Guard preprocessor untuk mencegah include ganda
#ifndef AVL_TREE_H
// Definisikan simbol AVL_TREE_H sebagai tanda file sudah di-include
#define AVL_TREE_H

// Include library string untuk tipe data std::string (menyimpan nama, kategori, asal produk)
#include <string>
// Include library vector untuk tipe data std::vector (menyimpan urutan data dinamis)
#include <vector>
// Include library algorithm untuk fungsi std::max, std::reverse, std::sort
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

// STRUCT: Node dalam AVL Tree, menyimpan data produk
struct AVLNode {
    // ID produk (key untuk sorting dalam tree)
    int         id;
    // Nama produk
    std::string name;
    // Kategori produk
    std::string category;
    // Harga produk dalam rupiah
    long        price;
    // Kota asal produk
    std::string origin;
    // Tinggi node (digunakan untuk calculate balance factor dan rebalancing)
    int         height;
    // Pointer ke child node kiri (produk dengan ID lebih kecil)
    AVLNode*    left;
    // Pointer ke child node kanan (produk dengan ID lebih besar)
    AVLNode*    right;

    // KONSTRUKTOR: Inisialisasi node baru dengan data produk
    AVLNode(int i, std::string n, std::string c, long p, std::string o)
        : id(i), name(n), category(c), price(p), origin(o),
        height(1), left(nullptr), right(nullptr) {}
};

// CLASS: Balanced binary search tree untuk penyimpanan produk by ID
// Menjamin O(log N) untuk semua operasi (insert, search, remove)
// melalui automatic rebalancing menggunakan rotasi
class AVLTree {
private:
    // Root pointer dari AVL tree (nullptr jika tree kosong)
    AVLNode* root;

    // ---- PRIVATE HELPER METHODS ----

    // Getter height: jika node null return 0, else return node->height
    int getHeight(AVLNode* n) const {
        // Ternary: jika n valid return height, else return 0
        return n ? n->height : 0;
    }

    // Hitung balance factor: height(left) - height(right)
    // AVL valid jika balance factor ∈ {-1, 0, +1}
    int getBalanceFactor(AVLNode* n) const {
        // Jika n null return 0
        return n ? getHeight(n->left) - getHeight(n->right) : 0;
    }

    // Update height node berdasarkan height children
    void updateHeight(AVLNode* n) {
        // Jika n valid
        if (n) 
            // Height = 1 + max(height_left, height_right)
            n->height = 1 + std::max(getHeight(n->left), getHeight(n->right));
    }

    // ---- ROTATION METHODS ----

    // Rotasi kanan: digunakan untuk Left-Left atau Left-Right case
    // Sebelum:        y           Sesudah:      x
    //               /            →           /   \
    //              x                        T1    y
    //             / \                            / \
    //            T1 T2                          T2 T3
    AVLNode* rotateRight(AVLNode* y) {
        // Save pointer ke left child dari y
        AVLNode* x  = y->left;
        // Save pointer ke right child dari x
        AVLNode* T2 = x->right;
        // Perform rotasi: x menjadi parent, y menjadi right child
        x->right = y;
        y->left  = T2;
        // Update height untuk kedua node (y dulu karena ada di depth lebih dalam)
        updateHeight(y);
        updateHeight(x);
        // Return x sebagai new subtree root
        return x;
    }

    // Rotasi kiri: digunakan untuk Right-Right atau Right-Left case
    // Sebelum:      x             Sesudah:        y
    //                \            →              / \
    //                 y                         x   T3
    //                / \                       / \
    //               T2 T3                     T1 T2
    AVLNode* rotateLeft(AVLNode* x) {
        // Save pointer ke right child dari x
        AVLNode* y  = x->right;
        // Save pointer ke left child dari y
        AVLNode* T2 = y->left;
        // Perform rotasi: y menjadi parent, x menjadi left child
        y->left  = x;
        x->right = T2;
        // Update height untuk kedua node
        updateHeight(x);
        updateHeight(y);
        // Return y sebagai new subtree root
        return y;
    }

    // Rebalance node berdasarkan balance factor
    // Jika |bf| > 1, perform rotasi untuk restore balance
    AVLNode* rebalance(AVLNode* node) {
        // Update height sebelum check balance factor
        updateHeight(node);
        // Hitung balance factor
        int bf = getBalanceFactor(node);

        // LEFT-HEAVY case
        // Jika bf > 1 dan left subtree balance factor >= 0 → Left-Left case
        if (bf > 1 && getBalanceFactor(node->left) >= 0)
            // Rotasi kanan cukup
            return rotateRight(node);

        // Jika bf > 1 dan left subtree balance factor < 0 → Left-Right case
        if (bf > 1 && getBalanceFactor(node->left) < 0) {
            // Rotasi kiri pada left child dulu
            node->left = rotateLeft(node->left);
            // Kemudian rotasi kanan pada node
            return rotateRight(node);
        }

        // RIGHT-HEAVY case
        // Jika bf < -1 dan right subtree balance factor <= 0 → Right-Right case
        if (bf < -1 && getBalanceFactor(node->right) <= 0)
            // Rotasi kiri cukup
            return rotateLeft(node);

        // Jika bf < -1 dan right subtree balance factor > 0 → Right-Left case
        if (bf < -1 && getBalanceFactor(node->right) > 0) {
            // Rotasi kanan pada right child dulu
            node->right = rotateRight(node->right);
            // Kemudian rotasi kiri pada node
            return rotateLeft(node);
        }

        // Node sudah seimbang, return as-is
        return node;
    }

    // ---- INSERT RECURSIVE ----

    // Recursive insert: return new subtree root
    AVLNode* insertRek(AVLNode* node, int id, const std::string& name,
                       const std::string& cat, long price, const std::string& origin) {
        // Base case: empty subtree, create new node
        if (node == nullptr)
            return new AVLNode(id, name, cat, price, origin);

        // Recursive case: ID kecil dari node, insert ke left
        if (id < node->id)
            node->left  = insertRek(node->left,  id, name, cat, price, origin);
        // Recursive case: ID besar dari node, insert ke right
        else if (id > node->id)
            node->right = insertRek(node->right, id, name, cat, price, origin);
        // ID sudah ada (duplicate key)
        else {
            // Update data produk dengan nilai baru
            node->name     = name;
            node->category = cat;
            node->price    = price;
            node->origin   = origin;
            // Return node tanpa perubahan struktur tree
            return node;
        }

        // Rebalance subtree dan return new subtree root
        return rebalance(node);
    }

    // ---- DELETE HELPER ----

    // Find node dengan nilai ID minimum di subtree
    // (digunakan untuk mencari in-order successor saat delete node dengan 2 children)
    AVLNode* findMin(AVLNode* node) const {
        // Traverse terus ke left sampai tidak ada left child
        while (node->left) node = node->left;
        // Return node dengan value paling kecil
        return node;
    }

    // ---- DELETE RECURSIVE ----

    // Recursive delete: return new subtree root
    AVLNode* deleteRek(AVLNode* node, int id) {
        // Base case: empty tree atau node not found
        if (node == nullptr) return nullptr;

        // Recursive case: target di left subtree
        if (id < node->id) {
            node->left  = deleteRek(node->left,  id);
        } 
        // Recursive case: target di right subtree
        else if (id > node->id) {
            node->right = deleteRek(node->right, id);
        } 
        // Node ditemukan (id == node->id)
        else {
            // Case 1: Node punya 0 atau 1 child
            if (!node->left || !node->right) {
                // Ambil child yang non-null (atau nullptr jika leaf)
                AVLNode* child = node->left ? node->left : node->right;
                // Deallocate node
                delete node;
                // Return child untuk menggantikan node di parent
                return child;
            }
            // Case 2: Node punya 2 children
            // Cari in-order successor (node dengan nilai paling kecil di right subtree)
            AVLNode* successor = findMin(node->right);
            // Copy data dari successor ke node
            node->id       = successor->id;
            node->name     = successor->name;
            node->category = successor->category;
            node->price    = successor->price;
            node->origin   = successor->origin;
            // Recursive delete successor dari right subtree
            node->right    = deleteRek(node->right, successor->id);
        }

        // Rebalance subtree dan return new subtree root
        return rebalance(node);
    }

    // ---- SEARCH RECURSIVE ----

    // Recursive search: traverse tree mencari node dengan ID
    AVLNode* searchRek(AVLNode* node, int id) const {
        // Base case: node null atau ID match
        if (node == nullptr || node->id == id) return node;
        // Recursive case: ID lebih kecil, search left
        if (id < node->id) return searchRek(node->left,  id);
        // Recursive case: ID lebih besar, search right
        else                return searchRek(node->right, id);
    }

    // ---- IN-ORDER TRAVERSAL ----

    // In-order recursive traversal: left → node → right
    // Menghasilkan urutan ascending by ID
    void inOrderRek(AVLNode* node, std::vector<AVLNode*>& result) const {
        // Base case: node null, return
        if (!node) return;
        // Recursive: traverse left subtree
        inOrderRek(node->left, result);
        // Add current node ke result
        result.push_back(node);
        // Recursive: traverse right subtree
        inOrderRek(node->right, result);
    }

    // ---- CLEAR/DELETE ALL ----

    // Recursive clear: post-order traversal dan delete semua node
    void clearRek(AVLNode* node) {
        // Base case: node null, return
        if (!node) return;
        // Recursive: delete left subtree
        clearRek(node->left);
        // Recursive: delete right subtree
        clearRek(node->right);
        // Delete current node
        delete node;
    }

    // ---- VISUALIZATION ----

    // Recursive tree visualization untuk debug/UI
    void visualizeRek(AVLNode* node, std::string prefix,
                      bool isLeft, std::string& out) const {
        // Base case: node null
        if (!node) return;
        // Add prefix untuk formatting
        out += prefix + (isLeft ? "├── " : "└── ");
        // Hitung balance factor
        int bf = getBalanceFactor(node);
        // Format: "ID:X [bf=Y] nama"
        out += "ID:" + std::to_string(node->id) +
               " [bf=" + std::to_string(bf) + "] " +
               node->name + "\n";
        // Recursive: visualize left subtree
        visualizeRek(node->left,  prefix + (isLeft ? "│   " : "    "), true,  out);
        // Recursive: visualize right subtree
        visualizeRek(node->right, prefix + (isLeft ? "│   " : "    "), false, out);
    }

public:
    // KONSTRUKTOR: Inisialisasi AVL tree kosong
    AVLTree() : root(nullptr) {}

    // DESTRUKTOR: Deallocate semua node
    ~AVLTree() { clearRek(root); }

    // PUBLIC METHOD: Insert atau update produk by ID — O(log N)
    void insert(int id, const std::string& name, const std::string& cat,
                long price, const std::string& origin) {
        // Call recursive insert dan update root
        root = insertRek(root, id, name, cat, price, origin);
    }

    // PUBLIC METHOD: Hapus produk berdasarkan ID — O(log N)
    void remove(int id) {
        // Call recursive delete dan update root
        root = deleteRek(root, id);
    }

    // PUBLIC METHOD: Cari produk berdasarkan ID — O(log N) dijamin
    // Return: pointer ke AVLNode jika ditemukan, nullptr jika tidak
    AVLNode* search(int id) const {
        // Call recursive search
        return searchRek(root, id);
    }

    // PUBLIC METHOD: Get semua produk dalam urutan ascending by ID
    // Return: vector pointer ke AVLNode (sorted by ID)
    std::vector<AVLNode*> getAllSortedById() const {
        // Initialize result vector
        std::vector<AVLNode*> result;
        // In-order traversal untuk get sorted order
        inOrderRek(root, result);
        // Return sorted result
        return result;
    }

    // PUBLIC METHOD: Check apakah tree kosong
    bool isEmpty() const { return root == nullptr; }

    // PUBLIC METHOD: Get tinggi tree (untuk analisis complexity)
    int getTreeHeight() const { return getHeight(root); }

    // PUBLIC METHOD: Clear semua node dalam tree
    void clear() {
        // Clear semua node
        clearRek(root);
        // Reset root ke nullptr
        root = nullptr;
    }

    // PUBLIC METHOD: Tampilkan visualisasi tree dalam format text
    // Berguna untuk debugging dan UI edukasi
    std::string getTreeVisualization() const {
        // Jika tree kosong
        if (!root) return "(AVL Tree kosong)";
        // Format header dengan height info
        std::string out = "=== AVL TREE (by ID Produk) — Height: " +
                          std::to_string(getTreeHeight()) + " ===\n";
        // Add root info dengan balance factor
        out += "Root: ID " + std::to_string(root->id) +
               " [bf=" + std::to_string(getBalanceFactor(root)) + "] " +
               root->name + "\n";
        // Visualize left subtree
        visualizeRek(root->left,  "", true,  out);
        // Visualize right subtree
        visualizeRek(root->right, "", false, out);
        // Add footer info tentang balance factor
        out += "\n[bf = balance factor; harus -1, 0, atau +1 pada setiap node]";
        return out;
    }

    // PUBLIC METHOD: Validasi apakah tree adalah valid AVL
    // (semua balance factor harus ∈ {-1, 0, +1})
    bool isValidAVL(AVLNode* node = nullptr, bool useRoot = true) const {
        // Jika useRoot true, mulai dari root
        if (useRoot) node = root;
        // Base case: node null adalah valid
        if (!node) return true;
        // Get balance factor
        int bf = getBalanceFactor(node);
        // Jika balance factor out of range, invalid
        if (bf < -1 || bf > 1) return false;
        // Recursively check left dan right subtree
        return isValidAVL(node->left, false) && isValidAVL(node->right, false);
    }
};

// End guard preprocessor
#endif // AVL_TREE_H