#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include "src/services/checkout_service.h"
#include "src/services/route_service.h"
#include "src/models/order.h"

using namespace std;

void printSeparator() {
    cout << string(60, '=') << endl;
}

void testCheckout() {
    printSeparator();
    cout << "TEST 1: CHECKOUT & ORDER QUEUE" << endl;
    printSeparator();

    CheckoutService checkout;

    // 1. Create Orders
    cout << "Creating 3 orders..." << endl;
    checkout.createOrder("Andi", "Bekasi", 500000);
    checkout.createOrder("Budi", "Bogor", 750000);
    checkout.createOrder("Cici", "Depok", 1200000);

    assert(checkout.getQueue().getSize() == 3);
    
    cout << "\nCurrent Queue:" << endl;
    checkout.displayOrders();

    // 2. Process Orders
    cout << "\nProcessing Order 1 (Step 1: Menunggu -> Diproses)..." << endl;
    checkout.processNextOrder();
    assert(checkout.getQueue().front()->status == "Diproses");

    cout << "Processing Order 1 (Step 2: Diproses -> Selesai & Dequeued)..." << endl;
    checkout.processNextOrder();
    assert(checkout.getQueue().getSize() == 2);
    assert(checkout.getQueue().front()->namaPemesan == "Budi");

    cout << "\n[OK] Checkout & Queue tests passed!" << endl;
}

void testRoute() {
    printSeparator();
    cout << "TEST 2: GRAPH & ROUTE (BFS / DFS)" << endl;
    printSeparator();

    RouteService route;

    // 1. Display Graph
    cout << "Current Network Structure:" << endl;
    route.displayNetwork();

    // 2. BFS Shortest Path
    cout << "\nFinding shortest route from Jakarta to Bogor..." << endl;
    vector<string> path = route.findDeliveryRoute("Bogor");
    
    cout << "Route: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i] << (i < path.size() - 1 ? " -> " : "");
    }
    cout << endl;

    assert(path.size() == 3);
    assert(path[0] == "Jakarta");
    assert(path[2] == "Bogor");

    // 3. DFS Audit
    cout << "\nAuditing network (DFS Traversal)..." << endl;
    vector<string> audit = route.auditNetwork();
    
    cout << "Traversal order: ";
    for (size_t i = 0; i < audit.size(); ++i) {
        cout << audit[i] << (i < audit.size() - 1 ? ", " : "");
    }
    cout << endl;

    assert(audit.size() == 5);

    cout << "\n[OK] Graph & Route tests passed!" << endl;
}

int main() {
    try {
        testCheckout();
        testRoute();
        printSeparator();
        cout << "SUCCESS: ALL CHECKOUT & ROUTE MODULE TESTS PASSED!" << endl;
        printSeparator();
    } catch (const exception& e) {
        cerr << "FATAL ERROR: " << e.what() << endl;
        return 1;
    }
    return 0;
}
