

#ifndef _ROLLINGBALLROUTING_H_
#define _ROLLINGBALLROUTING_H_

#include <map>
#include "VirtualRouting.h"
#include "RollingBallRoutingPacket_m.h"
#include "GeoMathHelper.h"


#define DEFAULT_ROLLINGBALL_TIMEOUT   200.0
/* the default time out period is 200.0 sec
   If the hello messge was not received during this period
   the entry in the neighbor list may be deleted 
   */

// if something is wrong, will retry sending HELLO message ROLLINGBALL_RETRY_HELLO_DELAY second later
#define ROLLINGBALL_RETRY_HELLO_DELAY 1

using namespace std;



enum RollingBallRoutingTimers {
};

class RollingBallRouting: public VirtualRouting {
  private:

    static int nextId;
    // Parameters
    int RollingBallSetupFrameOverhead;	// in bytes
    bool collectTraceInfo;
    int currentSequenceNumber;
    double helloInterval;
    double activeRouteTimeout; //in s

    // RollingBallRouting-related member variables
    int totalSNnodes;
    int packetsPerNode;
    bool isSink;		//is a .ned file parameter of the Application module
    int seqHello;

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
    void processDataPacketFromMacLayer(RollingBallPacket*);

    int getNextHopRollingBall(RollingBallPacket*);              // RollingBall forwarding mode
    Point getNeighborLocation(int);
};

#endif				//ROLLINGBALLROUTINGMODULE
