

#ifndef _BOUNDHOLEROUTING_H_
#define _BOUNDHOLEROUTING_H_

#include <map>
#include "VirtualRouting.h"
#include "GpsrRoutingControl_m.h"
#include "BoundholeRoutingPacket_m.h"
#include "GeoMathHelper.h"


#define DEFAULT_BOUNDHOLE_TIMEOUT   200.0
/* the default time out period is 200.0 sec
   If the hello messge was not received during this period
   the entry in the neighbor list may be deleted 
   */

// if something is wrong, will retry sending HELLO message BOUNDHOLE_RETRY_HELLO_DELAY second later
#define BOUNDHOLE_RETRY_HELLO_DELAY 1

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


enum BoundholeRoutingTimers {
  BOUNDHOLE_HELLO_MSG_REFRESH_TIMER = 0,
  BOUNDHOLE_HELLO_MSG_EXPIRE_TIMER  = 1,
};

class BoundholeRouting: public VirtualRouting {
  private:

    static int nextId;
    // Parameters
    int BoundholeSetupFrameOverhead;	// in bytes
    double netSetupTimeout;
    bool collectTraceInfo;
    int currentSequenceNumber;
    double helloInterval;
    double activeRouteTimeout; //in s

    // BoundholeRouting-related member variables
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
    void processDataPacketFromMacLayer(BoundholePacket*);

    void updateNeighborTable(int, int, Point); // add a possible new neighbor
    int getNextHopBoundhole(BoundholePacket*);              // Boundhole forwarding mode
    Point getNeighborLocation(int);
};

#endif				//BOUNDHOLEROUTINGMODULE
