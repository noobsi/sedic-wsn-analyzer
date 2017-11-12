

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


#ifndef _GREEDYROUTING_H_
#define _GREEDYROUTING_H_

#include <map>
#include "VirtualRouting.h"
#include "GpsrRoutingControl_m.h"
#include "GreedyRoutingPacket_m.h"
#include "GeoMathHelper.h"


#define DEFAULT_GREEDY_TIMEOUT   200.0
/* the default time out period is 200.0 sec
   If the hello messge was not received during this period
   the entry in the neighbor list may be deleted 
   */

// if something is wrong, will retry sending HELLO message GREEDY_RETRY_HELLO_DELAY second later
#define GREEDY_RETRY_HELLO_DELAY 1

using namespace std;

struct NeighborRecord {
  int id;      // the node's ID
  Point location;
  double ts;   //the last time stamp of the hello msg from it
  int timesRx;   

  NeighborRecord() {
    id = 0;
    ts = 0.0;
    timesRx = 0;
  }

};

struct sink {
  int id;          // the Sink's ID
  Point location;
};


enum GreedyRoutingTimers {
  GREEDY_HELLO_MSG_REFRESH_TIMER = 0,
  GREEDY_HELLO_MSG_EXPIRE_TIMER  = 1,
};

class GreedyRouting: public VirtualRouting {
  private:

    static int nextId;
    // Parameters
    int GreedySetupFrameOverhead;	// in bytes
    double netSetupTimeout;
    bool collectTraceInfo;
    int currentSequenceNumber;
    double helloInterval;
    double activeRouteTimeout; //in s

    // GreedyRouting-related member variables
    int self;         // the node's ID
    Point selfLocation;
    bool isCoordinateSet; // to know whether the node's position has been set or not
    int totalSNnodes;
    int packetsPerNode;
    bool isSink;		//is a .ned file parameter of the Application module
    sink mySink; 
    int seqHello;
    vector<NeighborRecord> neighborTable;
    ResourceManager *resourceManager;

  protected:

    void startup();
    void finishSpecific();
    void fromApplicationLayer(cPacket *, const char *);
    void fromMacLayer(cPacket *, int, double, double);
    void handleNetworkControlCommand(cMessage *);
    void sendTopologySetupPacket();
    void timerFiredCallback(int);
    void processBufferedPacket();

    void sendHelloMessage();
    void processDataPacketFromMacLayer(GreedyPacket*);

    void updateNeighborTable(int, int, Point); // add a possible new neighbor
    int getNextHopGreedy(GreedyPacket*);              // Greedy forwarding mode
    Point getNeighborLocation(int);
};

#endif				//GREEDYROUTINGMODULE
