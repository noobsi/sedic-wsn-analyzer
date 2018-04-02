
#include "GlobalLocationService.h"

cModule *GlobalLocationService::networkModule;
bool GlobalLocationService::initialized = false;
std::vector<Point> GlobalLocationService::locations;
std::vector<std::vector<NeighborRecord>> GlobalLocationService::neighborTables;
const double range = 40;

void GlobalLocationService::initialize(cModule *module) {
  if (!initialized) {
    networkModule = module;
    initialized = true;

    int numNodes = (int) module->par("numNodes").longValue();
    locations.assign(numNodes, Point());
    neighborTables.assign(numNodes, std::vector<NeighborRecord>());
    for (int i = 0; i < numNodes; i++) {
      double xCoor = networkModule->getSubmodule("node", i)->par("xCoor").doubleValue();
      double yCoor = networkModule->getSubmodule("node", i)->par("yCoor").doubleValue();
      locations[i] = Point(xCoor, yCoor);
    }

    for (int i = 0; i < numNodes; i++) {
      for (int j = i + 1; j < numNodes; j++) {
        if (G::distance(locations[i], locations[j]) < range) {
          neighborTables[i].push_back(NeighborRecord(j, locations[j]));
          neighborTables[j].push_back(NeighborRecord(i, locations[i]));
        }
      }
    }
  }
}
std::vector<NeighborRecord> GlobalLocationService::getNeighborTable(int id) {
  return neighborTables[id];
}

Point GlobalLocationService::getLocation(int id) {
  return locations[id];
}
