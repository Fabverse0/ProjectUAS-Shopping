#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include "src/services/checkout_service.h"
#include "src/services/route_service.h"
#include "src/models/order.h"

void printSeparator() {
    std::cout << std::string(60, '=') << std::endl;
}

void testCheckout() {
    printSeparator();
    std::cout << "TEST 1: CHECKOUT & ORDER QUEUE" << std::endl;
    printSeparator();

    CheckoutService checkout;

    // 1. Create Orders
    std::cout << "Creating 3 orders..." << std::endl;
    checkout.createOrder("Andi", "Bekasi", 500000);
    checkout.createOrder("Budi", "Bogor", 750000);
    checkout.createOrder("Cici", "Depok", 1200000);

    assert(checkout.getQueue().getSize() == 3);
    
    std::cout << "\nCurrent Queue:" << std::endl;
    checkout.displayOrders();

    // 2. Process Orders
    std::cout << "\nProcessing Order 1 (Step 1: Menunggu -> Diproses)..." << std::endl;
    checkout.processNextOrder();
    assert(checkout.getQueue().front()->status == "Diproses");

    std::cout << "Processing Order 1 (Step 2: Diproses -> Selesai & Dequeued)..." << std::endl;
    checkout.processNextOrder();
    assert(checkout.getQueue().getSize() == 2);
    assert(checkout.getQueue().front()->namaPemesan == "Budi");

    std::cout << "\n[OK] Checkout & Queue tests passed!" << std::endl;
}

void testRoute() {
    printSeparator();
    std::cout << "TEST 2: GRAPH & ROUTE (BFS / DFS)" << std::endl;
    printSeparator();

    RouteService route;

    // 1. Display Graph
    std::cout << "Current Network Structure:" << std::endl;
    route.displayGraph();

    // 2. BFS Shortest Path
    std::cout << "\nFinding shortest route from Jakarta to Bogor..." << std::endl;
    std::vector<std::string> path = route.findDeliveryRoute("Bogor");
    
    std::cout << "Route: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i < path.size() - 1 ? " -> " : "");
    }
    std::cout << std::endl;

    assert(path.size() == 3);
    assert(path[0] == "Jakarta");
    assert(path[2] == "Bogor");

    // 3. DFS Audit
    std::cout << "\nAuditing network (DFS Traversal)..." << std::endl;
    std::vector<std::string> audit = route.auditNetwork();
    
    std::cout << "Traversal order: ";
    for (size_t i = 0; i < audit.size(); ++i) {
        std::cout << audit[i] << (i < audit.size() - 1 ? ", " : "");
    }
    std::cout << std::endl;

    assert(audit.size() == 5);

    std::cout << "\n[OK] Graph & Route tests passed!" << std::endl;
}

void testEdgeCases() {
    printSeparator();
    std::cout << "TEST 3: EDGE CASES" << std::endl;
    printSeparator();

    // 1. Process empty queue
    CheckoutService emptyCheckout;
    std::cout << "Testing process empty queue..." << std::endl;
    emptyCheckout.processNextOrder();
    assert(emptyCheckout.getQueue().getSize() == 0);

    // 2. BFS destination not found
    RouteService route;
    std::cout << "Testing invalid destination..." << std::endl;
    std::vector<std::string> invalidPath = route.findDeliveryRoute("Bandung");
    assert(invalidPath.empty());

    // 3. Normal route to same city / Jakarta
    std::cout << "Testing route Jakarta to Jakarta..." << std::endl;
    std::vector<std::string> sameCityPath = route.findDeliveryRoute("Jakarta");
    assert(!sameCityPath.empty());
    assert(sameCityPath[0] == "Jakarta");

    std::cout << "\n[OK] Edge case tests passed!" << std::endl;
}


int main() {
    try {
        testCheckout();
        testRoute();
        testEdgeCases();
        
        printSeparator();
        std::cout << "SUCCESS: ALL CHECKOUT & ROUTE MODULE TESTS PASSED!" << std::endl;
        printSeparator();
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
