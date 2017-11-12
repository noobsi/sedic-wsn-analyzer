/*********************************************************/
/*  Copyright (c) 2011. University of Pau, France        */
/*  LIUPPA Laboratory, T2I Team                          */
/*                                                       */
/*  Permission to use, copy, modify and distribute this  */
/*  code, without fee, and without written agreement is  */
/*  hereby granted, provided that the above copyright    */
/*  notice and the authors appear in all copies          */
/*                                                       */
/*  GREEDY Routing Protocol - only greedy implemented      */
/*  Version:  1.0                                        */
/*  Authors: Diop Mamour <serignemamour.diop@gmail.com>  */
/*           Congduc Pham <congduc.pham@univ-pau.fr>     */
/*********************************************************/
#include "GreedyRouting.h"


Define_Module(GreedyRouting);

int GreedyRouting::nextId;

//================================================================
//    startup
//================================================================
void GreedyRouting::startup(){
  self = getParentModule()->getParentModule()->getIndex();
  isCoordinateSet = false;

  totalSNnodes = getParentModule()->getParentModule()->getParentModule()->par("numNodes");
	resourceManager = check_and_cast <ResourceManager*>(getParentModule()->getParentModule()->getParentModule()
	  ->getSubmodule("node", self)->getSubmodule("ResourceManager"));

  helloInterval = (double)par("helloInterval") / 1000.0;
  activeRouteTimeout = (double)par("activeRouteTimeout") / 1000.0;
  neighborTable.clear();
  neighborTable.reserve(totalSNnodes);
  seqHello = par("seqHello");
  mySink.id = -1;
  nextId = 0; // static member
}

//================================================================
//    timerFiredCallback
//================================================================
void GreedyRouting::timerFiredCallback(int index){

  switch(index){

    case GREEDY_HELLO_MSG_REFRESH_TIMER:
      {
        sendHelloMessage();
        break;  
      }

    case GREEDY_HELLO_MSG_EXPIRE_TIMER :
      {
        break;
      }

    default: break;
  }
}

//================================================================
//    processBufferedPacket
//================================================================

void GreedyRouting::processBufferedPacket(){
  while (!TXBuffer.empty()) {
    toMacLayer(TXBuffer.front(), BROADCAST_MAC_ADDRESS);
    TXBuffer.pop();
  }
}

//================================================================
//    fromApplicationLayer
//================================================================
void GreedyRouting::fromApplicationLayer(cPacket * pkt, const char *destination){

  GreedyPacket *dataPacket = new GreedyPacket("GREEDY routing data packet", NETWORK_LAYER_PACKET);

  encapsulatePacket(dataPacket, pkt);
  dataPacket->setGreedyPacketKind(GREEDY_DATA_PACKET);
  dataPacket->setSource(SELF_NETWORK_ADDRESS);
  dataPacket->setDestination(destination);


  if (string(destination).compare(BROADCAST_NETWORK_ADDRESS)==0) {
    toMacLayer(dataPacket, BROADCAST_MAC_ADDRESS);
    collectOutput("GREEDY Packets received", "DATA from Application (broadcast)");
    collectOutput("GREEDY Packets sent", "DATA (broadcast)");
    return;
  }


  if (mySink.id==-1) {
    delete dataPacket;
    return;
  }
  else {
    dataPacket->setDestLocation(mySink.location);
    dataPacket->setPacketId(nextId++);

    int nextHop = getNextHopGreedy(dataPacket);
    if (nextHop != -1) {
      trace() << "WSN_EVENT SEND packetId:" << dataPacket->getPacketId() << " source:" << dataPacket->getSource()
        << " destination:" << dataPacket->getDestination() << " current:" << self;
      trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resourceManager->getRemainingEnergy();
      toMacLayer(dataPacket, nextHop);
      collectOutput("GREEDY Packets received", "DATA from Application (unicast,greedy)");
      collectOutput("GREEDY Packets sent", "DATA (unicast,greedy)");
      return;
    }
    else {
      delete dataPacket;
    }
  }

}



//================================================================
//    fromMacLayer
//================================================================
void GreedyRouting::fromMacLayer(cPacket * pkt, int macAddress, double rssi, double lqi){

  GreedyPacket *netPacket = dynamic_cast <GreedyPacket*>(pkt);

  if (!netPacket)
    return;

  switch (netPacket->getGreedyPacketKind()) {

    // process hello msg
    case GREEDY_HELLO_MSG_PACKET:
      {
        collectOutput("GREEDY Packets received", "HELLO");

        Point helloLocation = netPacket->getHelloLocation();
        updateNeighborTable(atoi(netPacket->getSource()), seqHello, helloLocation);
        break;
      }

      // process sink address msg
    case GREEDY_SINK_ADDRESS_PACKET:
      { 
        //processSinkAddress(netPacket);
        break;
      }
      // process data packet
    case GREEDY_DATA_PACKET:
      { 

        collectOutput("GREEDY Packets received", "DATA from MAC");

        string dst(netPacket->getDestination());
        string src(netPacket->getSource());

        if ((dst.compare(BROADCAST_NETWORK_ADDRESS) == 0))
          trace() << "Received data from node " << src << " by broadcast";
//        else
//          /* trace() << "Received data from node " << src << ", final destination: " << dst; */
//          trace() << "RECEIVED " << self << " SOURCE " << src;

        processDataPacketFromMacLayer(netPacket);
        break;
      }

    default: return;
  }
}

//================================================================
//    finishSpecific
//================================================================
void GreedyRouting::finishSpecific() {
  trace() << "WSN_EVENT FINAL" << " id:" << self << " x:" << selfLocation.x() << " y:" << selfLocation.y() << " deathTime:-1";
}

//================================================================
//    sendHelloMsg
//================================================================
void GreedyRouting::sendHelloMessage(){

  GreedyPacket *helloMsg = new GreedyPacket("GREEDY hello message packet", NETWORK_LAYER_PACKET);
  helloMsg->setGreedyPacketKind(GREEDY_HELLO_MSG_PACKET);
  helloMsg->setHelloLocation(selfLocation);
  helloMsg->setSource(SELF_NETWORK_ADDRESS);
  helloMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
  toMacLayer(helloMsg, BROADCAST_MAC_ADDRESS);
  collectOutput("GREEDY Packets sent", "HELLO");

  seqHello++;
  setTimer(GREEDY_HELLO_MSG_REFRESH_TIMER, helloInterval);
}

//================================================================
//    processDataPacket
//================================================================
void GreedyRouting::processDataPacketFromMacLayer(GreedyPacket* pkt){

  string dst(pkt->getDestination());
  string src(pkt->getSource());

  // if the node is the destination
  if ((dst.compare(SELF_NETWORK_ADDRESS) == 0) || (self == mySink.id)) {
    trace() << "WSN_EVENT RECEIVE packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resourceManager->getRemainingEnergy();
    collectOutput("GREEDY Packets received", "final from MAC");
#ifdef DEBUG_OUTPUT_LEVEL2		
    collectOutput("GREEDY Packets received", atoi(src.c_str()), "final from MAC");
#endif		
    toApplicationLayer(pkt->decapsulate());
    return;
  } 

  // if the node is the destination by broadcast, we do not forward it
  if ((dst.compare(BROADCAST_NETWORK_ADDRESS) == 0)) {
    trace() << "Received data (routing broadcast) from MAC, send data to application layer. Source node: " << src;
    collectOutput("GREEDY Packets received", "broadcast from MAC");
#ifdef DEBUG_OUTPUT_LEVEL2
    collectOutput("GREEDY Packets received", atoi(src.c_str()), "broadcast from MAC");
#endif
    toApplicationLayer(pkt->decapsulate());
    return;
  }


  // duplicate the packet because we are going to forward it
  GreedyPacket *netPacket = pkt->dup();
  int nextHop = getNextHopGreedy(netPacket);
  if (nextHop != -1) {
    trace() << "WSN_EVENT FORWARD packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resourceManager->getRemainingEnergy();
    toMacLayer(netPacket, nextHop);
    collectOutput("GREEDY Packets received", "DATA from Application (unicast,greedy)");
    collectOutput("GREEDY Packets sent", "DATA (unicast,greedy)");
    return;
  }
  else {
    trace() << "WSN_EVENT DROP packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    // TODO - drop packet, cannot send
    delete netPacket;
  }
}

//================================================================
//    updateNeighborTable
//================================================================
void GreedyRouting::updateNeighborTable(int nodeID, int theSN, Point nodeLocation) {

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
//    newRec.x = x_node;
//    newRec.y = y_node;
    newRec.location = nodeLocation;
    newRec.ts = simTime().dbl();
    newRec.timesRx = 1;

    neighborTable.push_back(newRec);

    // print the last item
    /* trace() << "New neighbor for node " << self << " : node "<< neighborTable[(int)neighborTable.size()-1].id; */

    /*trace() << "id:" << neighborTable[pos].id << " x:" << neighborTable[pos].x << " y:" << neighborTable[pos].y << " timestamp:" << neighborTable[pos].ts << " times Rx:" << neighborTable[pos].timesRx << " received packets:" << neighborTable[pos].receivedPackets << endl;
    */
  } else {

    //it's an already known neighbor
//    neighborTable[pos].x = x_node; // updating of location
//    neighborTable[pos].y = y_node;
    neighborTable[pos].location = nodeLocation;
    neighborTable[pos].ts = simTime().dbl();
    neighborTable[pos].timesRx++;
  }

  /* trace() << "Neighbors list of node " << self << ":"; */

  tblSize = (int)neighborTable.size();

  /* for (int j = 0; j < tblSize; j++) */
    /* trace() << "Node " << neighborTable[j].id << "(" << neighborTable[j].x << "," << neighborTable[j].y << */
      /* "). Received " << neighborTable[j].timesRx << " HELLO from it."; */

  /* trace() << "--------------"; */
}


//================================================================
//   getNextHopGreedy - return next hop of greedy if found one,
//   other wise set the mode to GREEDY and find next GREEDY hop
//================================================================
int GreedyRouting::getNextHopGreedy(GreedyPacket* dataPacket){

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


Point GreedyRouting::getNeighborLocation(int id) {
  for (auto &n: neighborTable) {
    if (n.id == id) {
      return n.location;
    }
  }

  return Point(); // default
}
// will handle interaction between the application layer and the GPRS module in order to pass parameters such as
// the node's position
void GreedyRouting::handleNetworkControlCommand(cMessage *msg) {

  GpsrRoutingControlCommand *cmd = check_and_cast <GpsrRoutingControlCommand*>(msg);
  switch (cmd->getGpsrRoutingCommandKind()) {

    case SET_GPSR_NODE_POS: 
      {

        double selfX = cmd->getDouble1();
        double selfY = cmd->getDouble2();
        selfLocation = Point(selfX, selfY);
        isCoordinateSet = true;

        // normally, this is the first HELLO message
        if (isCoordinateSet) {
          sendHelloMessage();
        }

        break;
      }

    case SET_GPSR_SINK_POS: 
      {

        double x = cmd->getDouble1();
        double y = cmd->getDouble2();
        mySink.id = cmd->getInt1();
        mySink.location = Point(x, y);

//        trace() << "Application layer has set sink's position for next transferts SINK_" << mySink.id << "(" << mySink.x << ","
//          << mySink.y << ")";

        break;
      }
  }
  // don't delete the message since it will get deleted by the VirtualRouting class
}
