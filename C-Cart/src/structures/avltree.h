#ifndef AVLTREE_H
#define AVLTREE_H

#include "../models/product.h"
#include <vector>

// Node untuk AVL Tree
struct AVLNode {
    Product data;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(Product p) : data(p), left(nullptr), right(nullptr), height(1) {}
};

// Struktur data AVL Tree untuk penyimpanan produk yang seimbang (berdasarkan ID)
class AVLTree {
private:
    AVLNode* root;

    // Fungsi pembantu
    int height(AVLNode* n);
    int getBalance(AVLNode* n);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode* node, Product product);
    AVLNode* deleteNode(AVLNode* root, int id);
    AVLNode* minValueNode(AVLNode* node);
    AVLNode* search(AVLNode* root, int id);

    void inorder(AVLNode* root, std::vector<Product>& products);
    void preorder(AVLNode* root, std::vector<Product>& products);
    void postorder(AVLNode* root, std::vector<Product>& products);
    void destroyTree(AVLNode* node);

public:
    AVLTree();
    ~AVLTree();

    // Menambah produk ke tree
    void insert(Product product);

    // Menghapus produk berdasarkan ID
    void deleteProduct(int id);

    // Mencari produk berdasarkan ID
    Product* search(int id);

    // Mendapatkan tinggi tree
    int getHeight();

    // Mendapatkan balance factor node root
    int getBalanceFactor();

    // Traversal (mengembalikan vector produk)
    std::vector<Product> inorderTraversal();
    std::vector<Product> preorderTraversal();
    std::vector<Product> postorderTraversal();
};

#endif // AVLTREE_H
