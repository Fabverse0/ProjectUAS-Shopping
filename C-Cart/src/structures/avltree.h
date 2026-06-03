#ifndef AVLTREE_H
#define AVLTREE_H

#include "../models/product.h"
#include <vector>

/**
 * @brief Node for AVL Tree
 */
struct AVLNode {
    Product data;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(Product p) : data(p), left(nullptr), right(nullptr), height(1) {}
};

/**
 * @brief AVL Tree data structure for balanced search operations.
 * Stores products ordered by ID.
 */
class AVLTree {
private:
    AVLNode* root;

    // Helper functions
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

    /**
     * @brief Insert a product into the tree
     */
    void insert(Product product);

    /**
     * @brief Delete a product by its ID
     */
    void deleteProduct(int id);

    /**
     * @brief Search for a product by its ID
     * @return Pointer to the product if found, nullptr otherwise
     */
    Product* search(int id);

    /**
     * @brief Get the height of the tree
     */
    int getHeight();

    /**
     * @brief Get the balance factor of the root node
     */
    int getBalanceFactor();

    /**
     * @brief Get all products using inorder traversal (sorted by ID)
     */
    std::vector<Product> inorderTraversal();

    /**
     * @brief Get all products using preorder traversal
     */
    std::vector<Product> preorderTraversal();

    /**
     * @brief Get all products using postorder traversal
     */
    std::vector<Product> postorderTraversal();
};

#endif // AVLTREE_H
