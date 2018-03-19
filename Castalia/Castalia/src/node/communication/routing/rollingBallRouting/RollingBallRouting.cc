#include "RollingBallRouting.h"

Define_Module(RollingBallRouting);

int RollingBallRouting::nextId;

void RollingBallRouting::startup(){
  seqHello = par("seqHello");
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


  if (string(destination).compare(BROADCAST_NETWORK_ADDRESS)==0) {
    toMacLayer(dataPacket, BROADCAST_MAC_ADDRESS);
    return;
  }


  dataPacket->setDestLocation(GlobalLocationService::getLocation(atoi(destination)));
  dataPacket->setPacketId(nextId++);

  int nextHop = getNextHopRollingBall(dataPacket);
  if (nextHop != -1) {
    trace() << "WSN_EVENT SEND packetId:" << dataPacket->getPacketId() << " source:" << dataPacket->getSource()
      << " destination:" << dataPacket->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resMgrModule->getRemainingEnergy();
    toMacLayer(dataPacket, nextHop);
    return;
  }
  else {
    delete dataPacket;
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
    delete netPacket;
  }
}

int RollingBallRouting::getNextHopRollingBall(RollingBallPacket* dataPacket){
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

  return nextHop;
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
