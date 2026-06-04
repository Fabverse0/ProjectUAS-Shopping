#ifndef ROUTE_SERVICE_H
#define ROUTE_SERVICE_H

#include "../structures/graph.h"
#include <string>
#include <vector>

/**
 * @brief Service class to calculate and provide delivery routes.
 * Acts as a bridge between UI and Graph structure.
 */
class RouteService {
private:
    Graph cityGraph;
    const std::string startCity = "Jakarta";

public:
    RouteService();

    /**
     * @brief Initialize graph with default cities and edges
     */
    void initializeDefaultGraph();

    /**
     * @brief Find the delivery route from Jakarta to destination
     * Uses BFS for shortest path
     */
    std::vector<std::string> findDeliveryRoute(std::string destination);

    /**
     * @brief Get audit of all connected cities
     * Uses DFS for traversal
     */
    std::vector<std::string> auditNetwork();

    /**
     * @brief Display the current city network
     */
    void displayGraph() const;
};

#endif // ROUTE_SERVICE_H
