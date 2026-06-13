#ifndef ROUTE_SERVICE_H
#define ROUTE_SERVICE_H

#include "../structures/graph.h"
#include <string>
#include <vector>

// Layanan untuk perhitungan dan penyediaan rute pengiriman
class RouteService {
private:
    Graph cityGraph;
    const std::string startCity = "Jakarta";

public:
    RouteService();

    // Inisialisasi peta default
    void initializeDefaultGraph();

    // Mencari rute pengiriman ke kota tujuan (BFS)
    std::vector<std::string> findDeliveryRoute(std::string destination);

    // Audit seluruh jaringan kota yang terhubung (DFS)
    std::vector<std::string> auditNetwork();

    // Menampilkan peta jaringan kota
    void displayNetwork() const;
};

#endif // ROUTE_SERVICE_H
