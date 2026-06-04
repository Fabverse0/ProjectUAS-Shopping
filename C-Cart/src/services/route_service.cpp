#include "route_service.h"
#include <iostream>

using namespace std;

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

vector<string> RouteService::findDeliveryRoute(string destination) {
    return cityGraph.bfsShortestRoute(startCity, destination);
}

vector<string> RouteService::auditNetwork() {
    return cityGraph.dfsTraversal(startCity);
}

void RouteService::displayNetwork() const {
    cityGraph.display();
}
