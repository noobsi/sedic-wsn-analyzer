#include "RollingBallRouting.h"

Define_Module(RollingBallRouting);

int RollingBallRouting::nextId;

void RollingBallRouting::startup(){
  nextId = 0; // static member
}

void RollingBallRouting::timerFiredCallback(int index){

  switch(index){
    default: break;
  }
}


void RollingBallRouting::processBufferedPacket(){
  while (!TXBuffer.empty()) {
    toMacLayer(TXBuffer.front(), BROADCAST_MAC_ADDRESS);
    TXBuffer.pop();
  }
}

void RollingBallRouting::fromApplicationLayer(cPacket * pkt, const char *destination){

  RollingBallPacket *dataPacket = new RollingBallPacket("ROLLING_BALL routing data packet", NETWORK_LAYER_PACKET);

  encapsulatePacket(dataPacket, pkt);
  dataPacket->setRollingBallPacketKind(ROLLINGBALL_DATA_PACKET);
  dataPacket->setSource(SELF_NETWORK_ADDRESS);
  dataPacket->setDestination(destination);
  dataPacket->setRoutingMode(GREEDY_ROUTING);


  if (string(destination).compare(BROADCAST_NETWORK_ADDRESS)==0) {
    toMacLayer(dataPacket, BROADCAST_MAC_ADDRESS);
    return;
  }

  dataPacket->setDestLocation(GlobalLocationService::getLocation(atoi(destination)));
  dataPacket->setPacketId(nextId++);
  int nextHop = getNextHop(dataPacket);
  if (nextHop != -1) {
    trace() << "WSN_EVENT SEND packetId:" << dataPacket->getPacketId() << " source:" << dataPacket->getSource()
      << " destination:" << dataPacket->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resMgrModule->getRemainingEnergy();
    toMacLayer(dataPacket, nextHop);
    return;
  }
  else {
//    delete dataPacket;
  }

}



void RollingBallRouting::fromMacLayer(cPacket * pkt, int macAddress, double rssi, double lqi){
  RollingBallPacket *netPacket = dynamic_cast <RollingBallPacket*>(pkt);
  if (!netPacket)
    return;

  switch (netPacket->getRollingBallPacketKind()) {
    case ROLLINGBALL_DATA_PACKET:
      { 
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

void RollingBallRouting::finishSpecific() {
  trace() << "WSN_EVENT FINAL" << " id:" << self << " x:" << selfLocation.x() << " y:" << selfLocation.y() << " deathTime:-1";
}

void RollingBallRouting::processDataPacketFromMacLayer(RollingBallPacket* pkt){
  string dst(pkt->getDestination());
  string src(pkt->getSource());

  // if the node is the destination
  if ((dst.compare(SELF_NETWORK_ADDRESS) == 0)) {
    trace() << "WSN_EVENT RECEIVE packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resMgrModule->getRemainingEnergy();
    toApplicationLayer(pkt->decapsulate());
    return;
  } 

  // if the node is the destination by broadcast, we do not forward it
  if ((dst.compare(BROADCAST_NETWORK_ADDRESS) == 0)) {
    trace() << "Received data (routing broadcast) from MAC, send data to application layer. Source node: " << src;
    toApplicationLayer(pkt->decapsulate());
    return;
  }


  // duplicate the packet because we are going to forward it
  RollingBallPacket *netPacket = pkt->dup();
  int nextHop = getNextHopRollingBall(netPacket);
  if (nextHop != -1) {
    trace() << "WSN_EVENT FORWARD packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resMgrModule->getRemainingEnergy();
    toMacLayer(netPacket, nextHop);
    return;
  }
  else {
    trace() << "WSN_EVENT DROP packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
//    delete netPacket;
  }
}

int RollingBallRouting::getNextHopGreedy(RollingBallPacket* dataPacket){
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

  if (nextHop != -1) {
    return nextHop;
  } else {
    dataPacket->setRoutingMode(ROLLINGBALL_ROUTING);
    dataPacket->setStuckLocation(selfLocation);

    // compute first ball with radius = RADIO_RANGE/2
    double x1 = selfLocation.x(), y1 = selfLocation.y();
    double x2 = destLocation.x(), y2 = destLocation.y();
    double d = G::distance(selfLocation, destLocation);
    double centerX = x1 + (x2 - x1) * RADIO_RANGE / 2 / d;
    double centerY = y1 + (y2 - y1) * RADIO_RANGE / 2 / d;

    // set ball radius
    dataPacket->setBallCenter(Point(centerX, centerY));
    return getNextHopRollingBall(dataPacket);
  }

}

int RollingBallRouting::getNextHop(RollingBallPacket* dataPacket){
  switch (dataPacket->getRoutingMode()) {
    case GREEDY_ROUTING: return getNextHopGreedy(dataPacket);
    case ROLLINGBALL_ROUTING: return getNextHopRollingBall(dataPacket);
    default: throw cRuntimeError("Unkown routing mode");
  }
}

int RollingBallRouting::getNextHopRollingBall(RollingBallPacket* dataPacket){
  Point stuckLocation = dataPacket->getStuckLocation();
  Point destLocation = dataPacket->getDestLocation();
  if (G::distance(selfLocation, destLocation) < G::distance(stuckLocation, destLocation)) {
    dataPacket->setRoutingMode(GREEDY_ROUTING);
    return getNextHopGreedy(dataPacket);
  } else {
    int nextHop = -1;
    Point nextCenter;
    Point ballCenter = dataPacket->getBallCenter();
//     // respect to current node
//    double ballCenterAngle = G::norm(atan2(ballCenter.y() - selfLocation.y(), ballCenter.x() - selfLocation.x()));
//    double minAngle = 3 * M_PI;
//    for (auto &n: neighborTable) {
//      Point candidateCenter = nearestCenter(selfLocation, n.location, dataPacket->getBallCenter());
//      double candidateAngle = G::norm(atan2(candidateCenter.y() - selfLocation.y(), candidateCenter.x() - selfLocation.x()));
//      if (G::norm(candidateAngle - ballCenterAngle) < minAngle) {
//        minAngle = G::norm(candidateAngle - ballCenterAngle);
//        nextHop = n.id;
//        nextCenter = candidateCenter;
//      }
//    }

    nextHop = G::findNextHopRollingBall(selfLocation, ballCenter, RADIO_RANGE / 2, neighborTable, nextCenter);
    if (nextHop != -1) {
      dataPacket->setBallCenter(nextCenter);
    }

    return nextHop;
  }
}

Point RollingBallRouting::nearestCenter(Point pivot, Point next, Point center) {
  Point center1, center2;
  G::centers(pivot, next, RADIO_RANGE / 2, center1, center2);

  double pivotAngle = G::norm(atan2(pivot.y() - center1.y(), pivot.x() - center1.x()));
  double nextAngle = G::norm(atan2(next.y() - center1.y(), next.x() - center1.x()));
  double diffCCWAngle = G::norm(pivotAngle - nextAngle);

  if (diffCCWAngle < M_PI) {
    return center1;
  } else {
    return center2;
  }
}


Point RollingBallRouting::getNeighborLocation(int id) {
  for (auto &n: neighborTable) {
    if (n.id == id) {
      return n.location;
    }
  }

  return Point(); // default
}
// will handle interaction between the application layer and the GPRS module in order to pass parameters such as
// the node's position
void RollingBallRouting::handleNetworkControlCommand(cMessage *msg) {
}
