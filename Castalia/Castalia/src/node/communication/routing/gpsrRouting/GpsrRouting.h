

#ifndef _GPSRROUTING_H_
#define _GPSRROUTING_H_

#include <map>
#include "VirtualRouting.h"
#include "GpsrRoutingPacket_m.h"
#include "GeoMathHelper.h"
#include "GlobalLocationService.h"


#define DEFAULT_GPSR_TIMEOUT   200.0   
#define GPSR_RETRY_HELLO_DELAY 1

using namespace std;

enum GpsrRoutingTimers {
  GPSR_HELLO_MSG_REFRESH_TIMER = 0,
  GPSR_HELLO_MSG_EXPIRE_TIMER  = 1,
};

class GpsrRouting: public VirtualRouting {
  private:

    static int nextId;
    // Parameters
    int GpsrSetupFrameOverhead;	// in bytes
    bool collectTraceInfo;
    int currentSequenceNumber;
    double helloInterval;
    double activeRouteTimeout; //in s

    // GpsrRouting-related member variables
    int self;         // the node's ID
    int totalSNnodes;
    int packetsPerNode;
    int seqHello;
//    vector<NeighborRecord> neighborTable;
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
