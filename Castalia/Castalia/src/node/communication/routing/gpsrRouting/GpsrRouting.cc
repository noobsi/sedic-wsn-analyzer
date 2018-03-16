#include "GpsrRouting.h"


Define_Module(GpsrRouting);

int GpsrRouting::nextId;

//================================================================
//    startup
//================================================================
void GpsrRouting::startup(){
  self = getParentModule()->getParentModule()->getIndex();
	resourceManager = check_and_cast <ResourceManager*>(getParentModule()->getParentModule()->getParentModule()
	  ->getSubmodule("node", self)->getSubmodule("ResourceManager"));
  helloInterval = (double)par("helloInterval") / 1000.0;
  neighborTable = GlobalLocationService::getNeighborTable(self);
  seqHello = par("seqHello");
  nextId = 0; // static member
}

//================================================================
//    timerFiredCallback
//================================================================
void GpsrRouting::timerFiredCallback(int index){

  switch(index){

    case GPSR_HELLO_MSG_REFRESH_TIMER: 
      {
        sendHelloMessage();
        break;  
      }

    case GPSR_HELLO_MSG_EXPIRE_TIMER :
      {
        break;
      }

    default: break;
  }
}

//================================================================
//    processBufferedPacket
//================================================================

void GpsrRouting::processBufferedPacket(){
  while (!TXBuffer.empty()) {
    toMacLayer(TXBuffer.front(), BROADCAST_MAC_ADDRESS);
    TXBuffer.pop();
  }
}

//================================================================
//    fromApplicationLayer
//================================================================
void GpsrRouting::fromApplicationLayer(cPacket * pkt, const char *destination){  

  GpsrPacket *dataPacket = new GpsrPacket("GPSR routing data packet", NETWORK_LAYER_PACKET);

  std::cout << "sending to " << destination;

  encapsulatePacket(dataPacket, pkt);
  dataPacket->setGpsrPacketKind(GPSR_DATA_PACKET);
  dataPacket->setSource(SELF_NETWORK_ADDRESS);
  dataPacket->setDestination(destination);
  dataPacket->setRoutingMode(GPSR_GREEDY_ROUTING);


  if (string(destination).compare(BROADCAST_NETWORK_ADDRESS)==0) {
    toMacLayer(dataPacket, BROADCAST_MAC_ADDRESS);
    collectOutput("GPSR Packets received", "DATA from Application (broadcast)");
    collectOutput("GPSR Packets sent", "DATA (broadcast)");
    return;
  }

  // destination location
  dataPacket->setDestLocation(GlobalLocationService::getLocation(atoi(destination)));
  dataPacket->setPreviousLocation(Point()); // previous is unspecified
  dataPacket->setPreviousId(-1); // no previous node
  dataPacket->setPacketId(nextId++);

  int nextHop = getNextHop(dataPacket);
  if (nextHop != -1) {
    dataPacket->setPreviousLocation(selfLocation); // previous is unspecified
    dataPacket->setPreviousId(self); // no previous node
    trace() << "WSN_EVENT SEND packetId:" << dataPacket->getPacketId() << " source:" << dataPacket->getSource()
      << " destination:" << dataPacket->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resourceManager->getRemainingEnergy();
    toMacLayer(dataPacket, nextHop);
    collectOutput("GPSR Packets received", "DATA from Application (unicast,greedy)");
    collectOutput("GPSR Packets sent", "DATA (unicast,greedy)");
    return;
  }
  else {
    delete dataPacket;
  }

}



//================================================================
//    fromMacLayer
//================================================================
void GpsrRouting::fromMacLayer(cPacket * pkt, int macAddress, double rssi, double lqi){

  GpsrPacket *netPacket = dynamic_cast <GpsrPacket*>(pkt);

  if (!netPacket)
    return;

  switch (netPacket->getGpsrPacketKind()) {
      // process data packet
    case GPSR_DATA_PACKET:
      { 

        collectOutput("GPSR Packets received", "DATA from MAC");

        string dst(netPacket->getDestination());
        string src(netPacket->getSource());

        if ((dst.compare(BROADCAST_NETWORK_ADDRESS) == 0))
          trace() << "Received data from node " << src << " by broadcast";

        processDataPacketFromMacLayer(netPacket);
        break;
      }

    default: return;
  }
}

//================================================================
//    finishSpecific
//================================================================
void GpsrRouting::finishSpecific() {
  trace() << "WSN_EVENT FINAL" << " id:" << self << " x:" << selfLocation.x() << " y:" << selfLocation.y() << " deathTime:-1";
}

//================================================================
//    sendHelloMsg
//================================================================
void GpsrRouting::sendHelloMessage(){
}

//================================================================
//    processDataPacket
//================================================================
void GpsrRouting::processDataPacketFromMacLayer(GpsrPacket* pkt){

  string dst(pkt->getDestination());
  string src(pkt->getSource());

  // if the node is the destination
  if (dst.compare(SELF_NETWORK_ADDRESS) == 0) {
    trace() << "WSN_EVENT RECEIVE packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resourceManager->getRemainingEnergy();
    collectOutput("GPSR Packets received", "final from MAC");
#ifdef DEBUG_OUTPUT_LEVEL2		
    collectOutput("GPSR Packets received", atoi(src.c_str()), "final from MAC");
#endif		
    toApplicationLayer(pkt->decapsulate());
    return;
  } 

  // if the node is the destination by broadcast, we do not forward it
  if ((dst.compare(BROADCAST_NETWORK_ADDRESS) == 0)) {
    trace() << "Received data (routing broadcast) from MAC, send data to application layer. Source node: " << src;
    collectOutput("GPSR Packets received", "broadcast from MAC");
#ifdef DEBUG_OUTPUT_LEVEL2
    collectOutput("GPSR Packets received", atoi(src.c_str()), "broadcast from MAC");
#endif
    toApplicationLayer(pkt->decapsulate());
    return;
  }


  // duplicate the packet because we are going to forward it
  GpsrPacket *netPacket = pkt->dup();
  int nextHop = getNextHop(netPacket);
  if (nextHop != -1) {
    netPacket->setPreviousLocation(selfLocation);
    netPacket->setPreviousId(self);

    trace() << "WSN_EVENT FORWARD packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resourceManager->getRemainingEnergy();
    toMacLayer(netPacket, nextHop);
    collectOutput("GPSR Packets received", "DATA from Application (unicast,greedy)");
    collectOutput("GPSR Packets sent", "DATA (unicast,greedy)");
    return;
  }
  else {
    trace() << "WSN_EVENT DROP packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    delete netPacket;
  }
}

//================================================================
//    updateNeighborTable
//================================================================
void GpsrRouting::updateNeighborTable(int nodeID, int theSN, Point nodeLocation) {

  int pos = -1;
  int tblSize = (int) neighborTable.size();

  for (int i = 0; i < tblSize; i++)
    if (neighborTable[i].id == nodeID) {
      pos = i;
      break;
    }

  // it's a new neighbor
  if (pos == -1) {
    NeighborRecord newRec;
    newRec.id = nodeID;
    newRec.location = nodeLocation;
    neighborTable.push_back(newRec);
  } else {
    neighborTable[pos].location = nodeLocation;
  }

  tblSize = (int)neighborTable.size();
}


int GpsrRouting::getNextHop(GpsrPacket *dataPacket) {
  switch (dataPacket->getRoutingMode()) {
    case GPSR_GREEDY_ROUTING: return getNextHopGreedy(dataPacket);
    case GPSR_PERIMETER_ROUTING: return getNextHopPerimeter(dataPacket);
    default: throw cRuntimeError("Unkown routing mode");
  }
}

//================================================================
//   getNextHopGreedy - return next hop of greedy if found one,
//   other wise set the mode to GPSR and find next GPSR hop
//================================================================
int GpsrRouting::getNextHopGreedy(GpsrPacket* dataPacket){

  int nextHop = -1; double dist = 0;
  int tblSize = (int)neighborTable.size();
  Point destLocation = dataPacket->getDestLocation();
  double minDist = G::distance(selfLocation, destLocation);

  for (auto &neighbor: neighborTable) {
    dist = G::distance(destLocation, neighbor.location);

    if (dist < minDist) {
      minDist = dist;
      nextHop = neighbor.id;
    }
  }

  if (nextHop == -1) {
    trace() << "WSN_EVENT DEBUG Turn to perimeter mode at " << self;
    dataPacket->setRoutingMode(GPSR_PERIMETER_ROUTING);
    dataPacket->setPerimeterRoutingStartLocation(selfLocation);
    dataPacket->setPerimeterRoutingFaceLocation(selfLocation);
    nextHop = getNextHopPerimeterInit(dataPacket); // NOTE -> if next hop is always != -1
    dataPacket->setCurrentFaceFirstSender(self);
    dataPacket->setCurrentFaceFirstReceiver(nextHop);
  }

  return nextHop;
}

int GpsrRouting::getNextHopPerimeterInit(GpsrPacket* dataPacket) {
  int nextHop = rightHandForward(dataPacket, dataPacket->getDestLocation(), atoi(dataPacket->getDestination()));
  trace() << "WSN_EVENT DEBUG Determine first next hop perimeter: " << nextHop;
  return nextHop;
}

int GpsrRouting::rightHandForward(GpsrPacket* dataPacket, Point pivotLocation, int pivotId) {
  vector<NeighborRecord> planarNeighbors = getPlanarNeighbors();
  double bpivot = G::norm(atan2(selfLocation.y() - pivotLocation.y(), selfLocation.x() - pivotLocation.x()));
  double angleMin = 3 * M_PI;
  double nextHop = -1;
  bool backUp = false;

  for (auto &neighbor: planarNeighbors) {
    if (pivotId == neighbor.id) {
      backUp = true;
      continue;
    }
    Point neighborLocation = neighbor.location;
    double bneighbor = G::norm(atan2(selfLocation.y() - neighborLocation.y(), selfLocation.x() - neighborLocation.x()));
    double angle = G::norm(bneighbor - bpivot);

    if (angle < angleMin) {
      angleMin = angle;
      nextHop = neighbor.id;
    }
  }

  if (nextHop == -1 && backUp) {
    nextHop = pivotId; // where this come from
  }

  return nextHop;
}

vector<NeighborRecord> GpsrRouting::getPlanarNeighbors() {
  // RNG
  vector<NeighborRecord> planarNeighbors;
  for (auto &v: neighborTable) {
    bool ok = true;
    for (auto &w: neighborTable) {
      if (v.id == w.id) continue;
      double uv = G::distance(selfLocation, v.location);
      double uw = G::distance(selfLocation, w.location);
      double vw = G::distance(v.location, w.location);

      if (uv > max(uw, vw)) {
        ok = false;
      }
    }
    if (ok) {
      planarNeighbors.push_back(v);
    }
  }
  return planarNeighbors;
};


//================================================================
//    getNextHopPerimeter
//================================================================
int GpsrRouting::getNextHopPerimeter(GpsrPacket* dataPacket) {


  trace() << "WSN_EVENT DEBUG already in perimeter";
  int nextHop = -1;
  Point startLocation = dataPacket->getPerimeterRoutingStartLocation();
  Point destLocation = dataPacket->getDestLocation();
  if (G::distance(selfLocation, destLocation) < G::distance(startLocation, destLocation)) {
    dataPacket->setRoutingMode(GPSR_GREEDY_ROUTING);
    return getNextHopGreedy(dataPacket);
  } else {
    int proposedNextHop = rightHandForward(dataPacket, dataPacket->getPreviousLocation(), dataPacket->getPreviousId());
    trace() << "WSN_EVENT DEBUG first apply right hand forward, found: " << proposedNextHop;
    if (proposedNextHop != -1) {
      if (self == dataPacket->getCurrentFaceFirstSender() && nextHop == dataPacket->getCurrentFaceFirstReceiver()) {
        return -1; // we're encountering a loop, better drop the packet
      }
      proposedNextHop = faceChange(dataPacket, proposedNextHop);
    }

    trace() << "WSN_EVENT DEBUG after apply faceChange, got: " << proposedNextHop;

    return proposedNextHop;
  }
}


int GpsrRouting::faceChange(GpsrPacket* dataPacket, int proposedNextHop) {
  trace() << "WSN_EVENT DEBUG apply face change on " << proposedNextHop;
  Point intersection;
  Point proposedLocation = getNeighborLocation(proposedNextHop);
  Point startLocation = dataPacket->getPerimeterRoutingStartLocation();
  Point firstFaceLocation = dataPacket->getPerimeterRoutingFaceLocation();
  Point destLocation = dataPacket->getDestLocation();

  if (G::intersection(proposedLocation, selfLocation, startLocation, destLocation, intersection)) {
    if (G::distance(intersection, destLocation) < G::distance(firstFaceLocation, destLocation)) {
      proposedNextHop = rightHandForward(dataPacket, proposedLocation, proposedNextHop);
      if (proposedNextHop != -1) {
        proposedNextHop = faceChange(dataPacket, proposedNextHop);
      }
    }
  }

  return proposedNextHop;
}

Point GpsrRouting::getNeighborLocation(int id) {
  for (auto &n: neighborTable) {
    if (n.id == id) {
      return n.location;
    }
  }

  return Point(); // default
}
// will handle interaction between the application layer and the GPRS module in order to pass parameters such as
// the node's position
void GpsrRouting::handleNetworkControlCommand(cMessage *msg) {
}
