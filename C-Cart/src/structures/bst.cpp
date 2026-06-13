#include "bst.h"

using namespace std;

BST::BST() : root(nullptr) {}

BST::~BST() {
    destroyTree(root);
}

void BST::destroyTree(BSTNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

BSTNode* BST::insert(BSTNode* node, Product product) {
    if (node == nullptr) {
        return new BSTNode(product);
    }

    if (product.harga < node->data.harga) {
        node->left = insert(node->left, product);
    } else {
        node->right = insert(node->right, product);
    }

    return node;
}

void BST::insert(Product product) {
    root = insert(root, product);
}

void BST::inorder(BSTNode* node, vector<Product>& products) {
    if (node) {
        inorder(node->left, products);
        products.push_back(node->data);
        inorder(node->right, products);
    }
}

vector<Product> BST::inorderTraversal() {
    vector<Product> products;
    inorder(root, products);
    return products;
}
