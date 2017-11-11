

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


#ifndef _GPSRROUTING_H_
#define _GPSRROUTING_H_

#include <map>
#include "VirtualRouting.h"
#include "GpsrRoutingControl_m.h"
#include "GpsrRoutingPacket_m.h"
#include "GeoMathHelper.h"


#define DEFAULT_GPSR_TIMEOUT   200.0   
/* the default time out period is 200.0 sec
   If the hello messge was not received during this period
   the entry in the neighbor list may be deleted 
   */

// if something is wrong, will retry sending HELLO message GPSR_RETRY_HELLO_DELAY second later
#define GPSR_RETRY_HELLO_DELAY 1

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


enum GpsrRoutingTimers {
  GPSR_HELLO_MSG_REFRESH_TIMER = 0,
  GPSR_HELLO_MSG_EXPIRE_TIMER  = 1,
};

class GpsrRouting: public VirtualRouting {
  private:
    // Parameters
    int GpsrSetupFrameOverhead;	// in bytes
    double netSetupTimeout;
    bool collectTraceInfo;
    int currentSequenceNumber;
    double helloInterval;
    double activeRouteTimeout; //in s

    // GpsrRouting-related member variables
    int self;         // the node's ID
    Point selfLocation;
    bool isCoordinateSet; // to know whether the node's position has been set or not
    int totalSNnodes;
    int packetsPerNode;
    bool isSink;		//is a .ned file parameter of the Application module
    sink mySink; 
    int seqHello;
    vector<NeighborRecord> neighborTable;

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
    void processDataPacketFromMacLayer(GpsrPacket*);

    void updateNeighborTable(int, int, Point); // add a possible new neighbor
    int getNextHop(GpsrPacket*);
    int getNextHopGreedy(GpsrPacket*);              // Greedy forwarding mode
    int getNextHopPerimeter(GpsrPacket*);          // Perimeter mode
    int getNextHopPerimeterInit(GpsrPacket*);          // Perimeter first time in
    int rightHandForward(GpsrPacket*, Point, int);
    int faceChange(GpsrPacket*, int);
    Point getNeighborLocation(int);
    vector<NeighborRecord> getPlanarNeighbors();
};

#endif				//GPSRROUTINGMODULE
