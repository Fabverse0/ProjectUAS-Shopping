#include "avltree.h"
#include <algorithm>

using namespace std;

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

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode* AVLTree::insert(AVLNode* node, Product product) {
    if (node == nullptr)
        return new AVLNode(product);

    if (product.id < node->data.id)
        node->left = insert(node->left, product);
    else if (product.id > node->data.id)
        node->right = insert(node->right, product);
    else 
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Kasus LL
    if (balance > 1 && product.id < node->left->data.id)
        return rightRotate(node);

    // Kasus RR
    if (balance < -1 && product.id > node->right->data.id)
        return leftRotate(node);

    // Kasus LR
    if (balance > 1 && product.id > node->left->data.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Kasus RL
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
    if (root == nullptr)
        return root;

    if (id < root->data.id)
        root->left = deleteNode(root->left, id);
    else if (id > root->data.id)
        root->right = deleteNode(root->right, id);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            AVLNode* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;

            delete temp;
        } else {
            AVLNode* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data.id);
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

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

void AVLTree::inorder(AVLNode* root, vector<Product>& products) {
    if (root) {
        inorder(root->left, products);
        products.push_back(root->data);
        inorder(root->right, products);
    }
}

vector<Product> AVLTree::inorderTraversal() {
    vector<Product> products;
    inorder(root, products);
    return products;
}

void AVLTree::preorder(AVLNode* root, vector<Product>& products) {
    if (root) {
        products.push_back(root->data);
        preorder(root->left, products);
        preorder(root->right, products);
    }
}

vector<Product> AVLTree::preorderTraversal() {
    vector<Product> products;
    preorder(root, products);
    return products;
}

void AVLTree::postorder(AVLNode* root, vector<Product>& products) {
    if (root) {
        postorder(root->left, products);
        postorder(root->right, products);
        products.push_back(root->data);
    }
}

vector<Product> AVLTree::postorderTraversal() {
    vector<Product> products;
    postorder(root, products);
    return products;
}
