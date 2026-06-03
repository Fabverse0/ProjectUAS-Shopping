#ifndef PRODUCT_SERVICE_H
#define PRODUCT_SERVICE_H

#include "../models/product.h"
#include "../structures/avltree.h"
#include "../structures/bst.h"
#include <vector>

/**
 * @brief Service class to handle product-related operations.
 * Acts as a bridge between UI and data structures.
 */
class ProductService {
private:
    AVLTree catalog;
    
public:
    ProductService();

    /**
     * @brief Load 15 hardcoded products into the catalog
     */
    void loadHardcodedProducts();
    
    /**
     * @brief Get all products sorted by ID
     */
    std::vector<Product> getAllProducts();

    /**
     * @brief Find a product by its ID using AVL search
     */
    Product* findProductById(int id);

    /**
     * @brief Insert a new product
     */
    void insertProduct(Product p);

    /**
     * @brief Delete a product by ID
     */
    void deleteProduct(int id);

    /**
     * @brief Get all products sorted by price using BST
     */
    std::vector<Product> getProductsSortedByPrice();
    
    /**
     * @brief Binary Search implementation for product ID
     * @param products Sorted vector of products by ID
     * @param id The ID to search for
     * @return Pointer to the product if found, nullptr otherwise
     */
    Product* binarySearchById(const std::vector<Product>& products, int id);

    /**
     * @brief Demo function to insert a product that might trigger AVL balancing
     */
    void insertDemoProduct();

    /**
     * @brief Demo function to delete a product that might trigger AVL balancing
     */
    void deleteDemoProduct();

    // Traversal demos
    std::vector<Product> getInorder();
    std::vector<Product> getPreorder();
    std::vector<Product> getPostorder();
};

#endif // PRODUCT_SERVICE_H
