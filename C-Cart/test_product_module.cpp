#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include "src/services/product_service.h"
#include "src/models/product.h"

using namespace std;

void printSeparator() {
    cout << string(60, '=') << endl;
}

void testAVL() {
    printSeparator();
    cout << "TEST 1: AVL TREE (Primary Storage & Balancing)" << endl;
    printSeparator();
    
    ProductService service;
    
    // 1. Check Initial Hardcoded Products
    auto products = service.getAllProducts();
    cout << "Checking initial products (should be 15)..." << endl;
    cout << "Count: " << products.size() << endl;
    assert(products.size() == 15);

    // 2. Test Search by ID
    cout << "Searching for ID 101..." << endl;
    Product* p = service.findProductById(101);
    assert(p != nullptr);
    cout << "Found: " << p->nama << " [ID: " << p->id << "]" << endl;

    // 3. Test AVL Balancing (Demo)
    cout << "\nInserting Demo Products to trigger rotations..." << endl;
    service.insertDemoProduct(); 
    assert(service.getAllProducts().size() == 18);
    
    cout << "Traversal Demo (Inorder - Sorted by ID):" << endl;
    auto inorder = service.getInorder();
    for(const auto& prod : inorder) {
        if (prod.id >= 900) cout << "  - ID: " << prod.id << " | Name: " << prod.nama << endl;
    }

    // 4. Test Deletion
    cout << "\nDeleting Demo Products..." << endl;
    service.deleteDemoProduct();
    assert(service.getAllProducts().size() == 15);
    assert(service.findProductById(901) == nullptr);

    cout << "\n[OK] AVL Tree tests passed!" << endl;
}

void testBST() {
    printSeparator();
    cout << "TEST 2: BST (Sorting by Price)" << endl;
    printSeparator();
    
    ProductService service;
    cout << "Retrieving products sorted by price (BST Inorder)..." << endl;
    auto sorted = service.getProductsSortedByPrice();
    
    assert(sorted.size() == 15);
    
    cout << left << setw(30) << "Product Name" << " | " << "Price" << endl;
    cout << string(45, '-') << endl;
    
    for (size_t i = 0; i < sorted.size(); ++i) {
        cout << left << setw(30) << sorted[i].nama 
             << " | Rp " << fixed << setprecision(0) << sorted[i].harga << endl;
        
        if (i > 0) {
            assert(sorted[i-1].harga <= sorted[i].harga);
        }
    }
    
    cout << "\n[OK] BST Price Sorting tests passed!" << endl;
}

void testBinarySearch() {
    printSeparator();
    cout << "TEST 3: BINARY SEARCH (O(log n) Search)" << endl;
    printSeparator();
    
    ProductService service;
    auto products = service.getAllProducts(); 
    
    cout << "Performing binary search for ID 201..." << endl;
    Product* p = service.binarySearchById(products, 201);
    assert(p != nullptr);
    assert(p->nama == "iPhone 14 Pro");
    cout << "Found: " << p->nama << endl;
    
    cout << "Performing binary search for non-existent ID 9999..." << endl;
    p = service.binarySearchById(products, 9999);
    assert(p == nullptr);
    cout << "Result: Not Found (Correct)" << endl;
    
    cout << "\n[OK] Binary Search tests passed!" << endl;
}

int main() {
    try {
        testAVL();
        testBST();
        testBinarySearch();
        printSeparator();
        cout << "SUCCESS: ALL MODULE TESTS PASSED!" << endl;
        printSeparator();
    } catch (const exception& e) {
        cerr << "FATAL ERROR: " << e.what() << endl;
        return 1;
    }
    return 0;
}
