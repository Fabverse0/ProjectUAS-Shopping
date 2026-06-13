#ifndef BST_H
#define BST_H

#include "../models/product.h"
#include <vector>

// Node untuk BST
struct BSTNode {
    Product data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(Product p) : data(p), left(nullptr), right(nullptr) {}
};

// Struktur data Binary Search Tree (untuk sorting berdasarkan harga)
class BST {
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, Product product);
    void inorder(BSTNode* node, std::vector<Product>& products);
    void destroyTree(BSTNode* node);

public:
    BST();
    ~BST();

    // Menambah produk ke BST (berdasarkan harga)
    void insert(Product product);

    // Mendapatkan produk terurut (inorder traversal)
    std::vector<Product> inorderTraversal();
};

#endif // BST_H
