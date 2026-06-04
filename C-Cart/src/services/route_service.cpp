#include "route_service.h"
#include <iostream>

RouteService::RouteService() {
    initializeDefaultGraph();
}

void RouteService::initializeDefaultGraph() {
    cityGraph.addEdge("Jakarta", "Bekasi");
    cityGraph.addEdge("Jakarta", "Depok");
    cityGraph.addEdge("Depok", "Bogor");
    cityGraph.addEdge("Bekasi", "Tangerang");
    cityGraph.addEdge("Bekasi", "Bogor");
}

std::vector<std::string> RouteService::findDeliveryRoute(std::string destination) {
    return cityGraph.bfsShortestRoute(startCity, destination);
}

std::vector<std::string> RouteService::auditNetwork() {
    return cityGraph.dfsTraversal(startCity);
}

void RouteService::displayGraph() const {
    cityGraph.displayGraph();
}
