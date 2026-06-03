#include "product_service.h"

ProductService::ProductService() {
    loadHardcodedProducts();
}

void ProductService::loadHardcodedProducts() {
    insertProduct(Product(101, "Laptop Asus ROG", "Laptop", 15000000));
    insertProduct(Product(102, "Laptop HP Pavilion", "Laptop", 10000000));
    insertProduct(Product(103, "MacBook Air M2", "Laptop", 18000000));
    insertProduct(Product(201, "iPhone 14 Pro", "Smartphone", 20000000));
    insertProduct(Product(202, "Samsung S23 Ultra", "Smartphone", 19000000));
    insertProduct(Product(203, "Google Pixel 7", "Smartphone", 12000000));
    insertProduct(Product(301, "Sony WH-1000XM5", "Headphone", 5000000));
    insertProduct(Product(302, "Bose QuietComfort", "Headphone", 4500000));
    insertProduct(Product(303, "AirPods Max", "Headphone", 8000000));
    insertProduct(Product(401, "Keychron K2", "Keyboard", 1500000));
    insertProduct(Product(402, "Logitech G Pro X", "Keyboard", 2000000));
    insertProduct(Product(403, "Razer BlackWidow", "Keyboard", 2500000));
    insertProduct(Product(501, "Logitech MX Master 3", "Mouse", 1500000));
    insertProduct(Product(502, "Razer DeathAdder", "Mouse", 1000000));
    insertProduct(Product(503, "SteelSeries Rival 3", "Mouse", 500000));
}

std::vector<Product> ProductService::getAllProducts() {
    return catalog.inorderTraversal();
}

Product* ProductService::findProductById(int id) {
    return catalog.search(id);
}

void ProductService::insertProduct(Product p) {
    catalog.insert(p);
}

void ProductService::deleteProduct(int id) {
    catalog.deleteProduct(id);
}

std::vector<Product> ProductService::getProductsSortedByPrice() {
    BST priceTree;
    std::vector<Product> all = catalog.inorderTraversal();
    for (const auto& p : all) {
        priceTree.insert(p);
    }
    return priceTree.inorderTraversal();
}

Product* ProductService::binarySearchById(const std::vector<Product>& products, int id) {
    int left = 0;
    int right = products.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (products[mid].id == id)
            return const_cast<Product*>(&products[mid]);

        if (products[mid].id < id)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return nullptr;
}

void ProductService::insertDemoProduct() {
    // Insert products in a way that triggers rotations
    // For example, 30, 20, 10 (LL rotation) or 10, 20, 30 (RR rotation)
    // We use unique IDs for demo
    insertProduct(Product(901, "Demo Product A", "Demo", 100000));
    insertProduct(Product(902, "Demo Product B", "Demo", 100000));
    insertProduct(Product(903, "Demo Product C", "Demo", 100000));
}

void ProductService::deleteDemoProduct() {
    deleteProduct(901);
    deleteProduct(902);
    deleteProduct(903);
}

std::vector<Product> ProductService::getInorder() {
    return catalog.inorderTraversal();
}

std::vector<Product> ProductService::getPreorder() {
    return catalog.preorderTraversal();
}

std::vector<Product> ProductService::getPostorder() {
    return catalog.postorderTraversal();
}
