#ifndef BST_H
#define BST_H

#include "../models/product.h"
#include <vector>

/**
 * @brief Node for BST
 */
struct BSTNode {
    Product data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(Product p) : data(p), left(nullptr), right(nullptr) {}
};

/**
 * @brief Binary Search Tree data structure.
 * Used for sorting products by price.
 */
class BST {
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, Product product);
    void inorder(BSTNode* node, std::vector<Product>& products);
    void destroyTree(BSTNode* node);

public:
    BST();
    ~BST();

    /**
     * @brief Insert a product into the BST ordered by price
     */
    void insert(Product product);

    /**
     * @brief Get all products sorted by price using inorder traversal
     */
    std::vector<Product> inorderTraversal();
};

#endif // BST_H
