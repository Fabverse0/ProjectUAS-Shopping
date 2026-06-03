#include "avltree.h"
#include <algorithm>

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    destroyTree(root);
}

void AVLTree::destroyTree(AVLNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

int AVLTree::height(AVLNode* n) {
    if (n == nullptr) return 0;
    return n->height;
}

int AVLTree::getBalance(AVLNode* n) {
    if (n == nullptr) return 0;
    return height(n->left) - height(n->right);
}

AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

AVLNode* AVLTree::insert(AVLNode* node, Product product) {
    // 1. Perform normal BST insertion
    if (node == nullptr)
        return new AVLNode(product);

    if (product.id < node->data.id)
        node->left = insert(node->left, product);
    else if (product.id > node->data.id)
        node->right = insert(node->right, product);
    else // Duplicate IDs are not allowed in this implementation
        return node;

    // 2. Update height of this ancestor node
    node->height = 1 + std::max(height(node->left), height(node->right));

    // 3. Get the balance factor to check if it became unbalanced
    int balance = getBalance(node);

    // If unbalanced, then there are 4 cases

    // Left Left Case (LL)
    if (balance > 1 && product.id < node->left->data.id)
        return rightRotate(node);

    // Right Right Case (RR)
    if (balance < -1 && product.id > node->right->data.id)
        return leftRotate(node);

    // Left Right Case (LR)
    if (balance > 1 && product.id > node->left->data.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case (RL)
    if (balance < -1 && product.id < node->right->data.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insert(Product product) {
    root = insert(root, product);
}

AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

AVLNode* AVLTree::deleteNode(AVLNode* root, int id) {
    // 1. Perform standard BST delete
    if (root == nullptr)
        return root;

    if (id < root->data.id)
        root->left = deleteNode(root->left, id);
    else if (id > root->data.id)
        root->right = deleteNode(root->right, id);
    else {
        // Node with only one child or no child
        if ((root->left == nullptr) || (root->right == nullptr)) {
            AVLNode* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else // One child case
                *root = *temp; // Copy contents of the non-empty child

            delete temp;
        } else {
            // Node with two children: Get inorder successor
            AVLNode* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->data = temp->data;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->data.id);
        }
    }

    // If the tree had only one node then return
    if (root == nullptr)
        return root;

    // 2. Update height
    root->height = 1 + std::max(height(root->left), height(root->right));

    // 3. Get balance factor
    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void AVLTree::deleteProduct(int id) {
    root = deleteNode(root, id);
}

AVLNode* AVLTree::search(AVLNode* root, int id) {
    if (root == nullptr || root->data.id == id)
        return root;

    if (root->data.id < id)
        return search(root->right, id);

    return search(root->left, id);
}

Product* AVLTree::search(int id) {
    AVLNode* result = search(root, id);
    if (result) return &(result->data);
    return nullptr;
}

int AVLTree::getHeight() {
    return height(root);
}

int AVLTree::getBalanceFactor() {
    return getBalance(root);
}

void AVLTree::inorder(AVLNode* root, std::vector<Product>& products) {
    if (root) {
        inorder(root->left, products);
        products.push_back(root->data);
        inorder(root->right, products);
    }
}

std::vector<Product> AVLTree::inorderTraversal() {
    std::vector<Product> products;
    inorder(root, products);
    return products;
}

void AVLTree::preorder(AVLNode* root, std::vector<Product>& products) {
    if (root) {
        products.push_back(root->data);
        preorder(root->left, products);
        preorder(root->right, products);
    }
}

std::vector<Product> AVLTree::preorderTraversal() {
    std::vector<Product> products;
    preorder(root, products);
    return products;
}

void AVLTree::postorder(AVLNode* root, std::vector<Product>& products) {
    if (root) {
        postorder(root->left, products);
        postorder(root->right, products);
        products.push_back(root->data);
    }
}

std::vector<Product> AVLTree::postorderTraversal() {
    std::vector<Product> products;
    postorder(root, products);
    return products;
}
