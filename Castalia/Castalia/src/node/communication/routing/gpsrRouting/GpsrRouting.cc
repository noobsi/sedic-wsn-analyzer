/*********************************************************/
/*  Copyright (c) 2011. University of Pau, France        */
/*  LIUPPA Laboratory, T2I Team                          */
/*                                                       */
/*  Permission to use, copy, modify and distribute this  */
/*  code, without fee, and without written agreement is  */
/*  hereby granted, provided that the above copyright    */
/*  notice and the authors appear in all copies          */
/*                                                       */
/*  GPSR Routing Protocol - only greedy implemented      */
/*  Version:  1.0                                        */
/*  Authors: Diop Mamour <serignemamour.diop@gmail.com>  */
/*           Congduc Pham <congduc.pham@univ-pau.fr>     */
/*********************************************************/
#include "GpsrRouting.h"
#include "GeoMathHelper.h"


Define_Module(GpsrRouting);

//================================================================
//    startup
//================================================================
void GpsrRouting::startup(){
  self = getParentModule()->getParentModule()->getIndex();
  isCoordinateSet = false;

  totalSNnodes = getParentModule()->getParentModule()->getParentModule()->par("numNodes");

  helloInterval = (double)par("helloInterval") / 1000.0;
  activeRouteTimeout = (double)par("activeRouteTimeout") / 1000.0;
  neighborTable.clear();
  neighborTable.reserve(totalSNnodes);
  seqHello = par("seqHello");

  mySink.id = -1;
  mySink.x = 0;
  mySink.y = 0;
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


  if (mySink.id==-1) {
    delete dataPacket;
    return;
  }
  else {
    dataPacket->setDestX(mySink.x);
    dataPacket->setDestY(mySink.y);

    int nextHop = getNextHop(dataPacket);
    if (nextHop != -1) {
      toMacLayer(dataPacket, nextHop);
      collectOutput("GPSR Packets received", "DATA from Application (unicast,greedy)");
      collectOutput("GPSR Packets sent", "DATA (unicast,greedy)");
      return;
    }
    else {
      delete dataPacket;
    }
  }

}


int GpsrRouting::getNextHop(GpsrPacket *dataPacket) {
  switch (dataPacket->getRoutingMode()) {
    case GPSR_GREEDY_ROUTING: return getNextHopGreedy(dataPacket);
    case GPSR_PERIMETER_ROUTING: return getNextHopPerimeter(dataPacket);
    default: throw cRuntimeError("Unkown routing mode");
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

    // process hello msg
    case GPSR_HELLO_MSG_PACKET: 
      {
        collectOutput("GPSR Packets received", "HELLO");

        updateNeighborTable(atoi(netPacket->getSource()), seqHello, netPacket->getHelloX(),netPacket->getHelloY());
        break;
      }

      // process sink address msg
    case GPSR_SINK_ADDRESS_PACKET:
      { 
        //processSinkAddress(netPacket);
        break;
      }
      // process data packet
    case GPSR_DATA_PACKET:
      { 

        collectOutput("GPSR Packets received", "DATA from MAC");

        string dst(netPacket->getDestination());
        string src(netPacket->getSource());

        if ((dst.compare(BROADCAST_NETWORK_ADDRESS) == 0))
          trace() << "Received data from node " << src << " by broadcast";
        else
          /* trace() << "Received data from node " << src << ", final destination: " << dst; */
          trace() << "RECEIVED " << self << " SOURCE " << src;

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

}

//================================================================
//    sendHelloMsg
//================================================================
void GpsrRouting::sendHelloMessage(){

  GpsrPacket *helloMsg = new GpsrPacket("GPSR hello message packet", NETWORK_LAYER_PACKET);
  helloMsg->setGpsrPacketKind(GPSR_HELLO_MSG_PACKET);
  helloMsg->setHelloX(selfX);
  helloMsg->setHelloY(selfY);
  helloMsg->setSource(SELF_NETWORK_ADDRESS);
  helloMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
  toMacLayer(helloMsg, BROADCAST_MAC_ADDRESS);
  collectOutput("GPSR Packets sent", "HELLO");

  seqHello++;
  setTimer(GPSR_HELLO_MSG_REFRESH_TIMER, helloInterval);
}

//================================================================
//    processDataPacket
//================================================================
void GpsrRouting::processDataPacketFromMacLayer(GpsrPacket* pkt){

  string dst(pkt->getDestination());
  string src(pkt->getSource());

  // if the node is the destination
  if ((dst.compare(SELF_NETWORK_ADDRESS) == 0) || (self == mySink.id)) {
    trace() << "Received data for myself (routing unicast) from MAC, send data to application layer. Source node: " << src;
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
    toMacLayer(netPacket, nextHop);
    collectOutput("GPSR Packets received", "DATA from Application (unicast,greedy)");
    collectOutput("GPSR Packets sent", "DATA (unicast,greedy)");
    return;
  }
  else {
    // TODO - drop packet, cannot send
    delete netPacket;
  }
}

//================================================================
//    updateNeighborTable
//================================================================
void GpsrRouting::updateNeighborTable(int nodeID, int theSN, double x_node, double y_node) {

  int pos = -1;
  int tblSize = (int)neighborTable.size();

  for (int i = 0; i < tblSize; i++)
    if (neighborTable[i].id == nodeID) {
      pos = i;
      break;
    }

  // it's a new neighbor
  if (pos == -1) {
    GPSR_neighborRecord newRec;

    newRec.id = nodeID;
    newRec.x = x_node;
    newRec.y = y_node;
    newRec.ts = simTime().dbl();
    newRec.timesRx = 1;

    neighborTable.push_back(newRec);

    // print the last item
    /* trace() << "New neighbor for node " << self << " : node "<< neighborTable[(int)neighborTable.size()-1].id; */

    /*trace() << "id:" << neighborTable[pos].id << " x:" << neighborTable[pos].x << " y:" << neighborTable[pos].y << " timestamp:" << neighborTable[pos].ts << " times Rx:" << neighborTable[pos].timesRx << " received packets:" << neighborTable[pos].receivedPackets << endl;
    */
  } else {

    //it's an already known neighbor
    neighborTable[pos].x = x_node; // updating of location
    neighborTable[pos].y = y_node;
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
//   getNextHopGreedy
//================================================================
int GpsrRouting::getNextHopGreedy(GpsrPacket* dataPacket){



  int nextHop = -1; double dist = 0;
  int tblSize = (int)neighborTable.size();
  double destX = dataPacket->getDestX();
  double destY = dataPacket->getDestY();
  double minDist = G::distance(selfX, selfY, destX, destY);

  for (int i = 0; i < tblSize; i++) {
    dist = G::distance(neighborTable[i].x, neighborTable[i].y, destX, destY);

    if (dist < minDist) {
      minDist = dist;
      nextHop = neighborTable[i].id;
    }
  }

  return nextHop;
}

//================================================================
//    getNextHopPerimeter
//================================================================
int GpsrRouting::getNextHopPerimeter(GpsrPacket* dataPacket) {
  return -1; // TODO - implement this
}


// will handle interaction between the application layer and the GPRS module in order to pass parameters such as
// the node's position
void GpsrRouting::handleNetworkControlCommand(cMessage *msg) {

  GpsrRoutingControlCommand *cmd = check_and_cast <GpsrRoutingControlCommand*>(msg);
  switch (cmd->getGpsrRoutingCommandKind()) {

    case SET_GPSR_NODE_POS: 
      {

        selfX = cmd->getDouble1();
        selfY = cmd->getDouble2();
        isCoordinateSet = true;

        // normally, this is the first HELLO message
        if (isCoordinateSet) {
          sendHelloMessage();
        }

        break;
      }

    case SET_GPSR_SINK_POS: 
      {

        mySink.x = cmd->getDouble1();
        mySink.y = cmd->getDouble2();
        mySink.id = cmd->getInt1();

        trace() << "Application layer has set sink's position for next transferts SINK_" << mySink.id << "(" << mySink.x << ","
          << mySink.y << ")";

        break;
      }
  }
  // don't delete the message since it will get deleted by the VirtualRouting class
}
