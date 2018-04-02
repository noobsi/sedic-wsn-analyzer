

#ifndef _STABLEROUTING_H_
#define _STABLEROUTING_H_

#include <map>
#include "VirtualRouting.h"
#include "StableRoutingPacket_m.h"
#include "GeoMathHelper.h"


#define DEFAULT_STABLE_TIMEOUT   200.0
/* the default time out period is 200.0 sec
   If the hello messge was not received during this period
   the entry in the neighbor list may be deleted 
   */

// if something is wrong, will retry sending HELLO message STABLE_RETRY_HELLO_DELAY second later
#define STABLE_RETRY_HELLO_DELAY 1

using namespace std;



enum StableRoutingTimers {
  DISCOVER_HOLE_START = 1,
};

class StableRouting: public VirtualRouting {
  private:

    static int nextId;
    // Parameters
    int StableSetupFrameOverhead;	// in bytes
    double secondBallRadius;
    bool collectTraceInfo;
    map<int, int> smallestOriginatorId; // smallest originator id come from direction of a neighbor node (given by id)
    // StableRouting-related member variables
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
    void processDataPacketFromMacLayer(StablePacket*);
    void processDiscoverHolePacket(DiscoverHolePacket*);
    void processHole(DiscoverHolePacket*);

    int getNextHop(StablePacket*);              // Stable forwarding mode
    Point getNeighborLocation(int);
    Point nearestCenter(Point pivot, Point next, Point center);
};

#endif				//STABLEROUTINGMODULE
