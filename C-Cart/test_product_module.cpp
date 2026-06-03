#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include "src/services/product_service.h"
#include "src/models/product.h"

void printSeparator() {
    std::cout << std::string(60, '=') << std::endl;
}

void testAVL() {
    printSeparator();
    std::cout << "TEST 1: AVL TREE (Primary Storage & Balancing)" << std::endl;
    printSeparator();
    
    ProductService service;
    
    // 1. Check Initial Hardcoded Products
    auto products = service.getAllProducts();
    std::cout << "Checking initial products (should be 15)..." << std::endl;
    std::cout << "Count: " << products.size() << std::endl;
    assert(products.size() == 15);

    // 2. Test Search by ID
    std::cout << "Searching for ID 101..." << std::endl;
    Product* p = service.findProductById(101);
    assert(p != nullptr);
    std::cout << "Found: " << p->nama << " [ID: " << p->id << "]" << std::endl;

    // 3. Test AVL Balancing (Demo)
    std::cout << "\nInserting Demo Products to trigger rotations..." << std::endl;
    service.insertDemoProduct(); // Inserts 901, 902, 903
    assert(service.getAllProducts().size() == 18);
    
    std::cout << "Traversal Demo (Inorder - Sorted by ID):" << std::endl;
    auto inorder = service.getInorder();
    for(const auto& prod : inorder) {
        if (prod.id >= 900) std::cout << "  - ID: " << prod.id << " | Name: " << prod.nama << std::endl;
    }

    // 4. Test Deletion
    std::cout << "\nDeleting Demo Products..." << std::endl;
    service.deleteDemoProduct();
    assert(service.getAllProducts().size() == 15);
    assert(service.findProductById(901) == nullptr);

    std::cout << "\n[OK] AVL Tree tests passed!" << std::endl;
}

void testBST() {
    printSeparator();
    std::cout << "TEST 2: BST (Sorting by Price)" << std::endl;
    printSeparator();
    
    ProductService service;
    std::cout << "Retrieving products sorted by price (BST Inorder)..." << std::endl;
    auto sorted = service.getProductsSortedByPrice();
    
    assert(sorted.size() == 15);
    
    std::cout << std::left << std::setw(30) << "Product Name" << " | " << "Price" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    
    for (size_t i = 0; i < sorted.size(); ++i) {
        std::cout << std::left << std::setw(30) << sorted[i].nama 
                  << " | Rp " << std::fixed << std::setprecision(0) << sorted[i].harga << std::endl;
        
        if (i > 0) {
            assert(sorted[i-1].harga <= sorted[i].harga);
        }
    }
    
    std::cout << "\n[OK] BST Price Sorting tests passed!" << std::endl;
}

void testBinarySearch() {
    printSeparator();
    std::cout << "TEST 3: BINARY SEARCH (O(log n) Search)" << std::endl;
    printSeparator();
    
    ProductService service;
    auto products = service.getAllProducts(); // Already sorted by ID by AVL inorder
    
    std::cout << "Performing binary search for ID 201..." << std::endl;
    Product* p = service.binarySearchById(products, 201);
    assert(p != nullptr);
    assert(p->nama == "iPhone 14 Pro");
    std::cout << "Found: " << p->nama << std::endl;
    
    std::cout << "Performing binary search for non-existent ID 9999..." << std::endl;
    p = service.binarySearchById(products, 9999);
    assert(p == nullptr);
    std::cout << "Result: Not Found (Correct)" << std::endl;
    
    std::cout << "\n[OK] Binary Search tests passed!" << std::endl;
}

int main() {
    try {
        testAVL();
        testBST();
        testBinarySearch();
        printSeparator();
        std::cout << "SUCCESS: ALL MODULE TESTS PASSED!" << std::endl;
        printSeparator();
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
