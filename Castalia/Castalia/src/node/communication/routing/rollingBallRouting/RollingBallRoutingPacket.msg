
cplusplus {{
#include "RoutingPacket_m.h"
#include "GeoMathHelper.h"
}}

class noncobject Point;

class RoutingPacket;

enum RollingBallForwardingMode {
    GREEDY_ROUTING = 0;
    ROLLINGBALL_ROUTING = 1;
};

enum RollingBallPacketDef {
    ROLLINGBALL_DATA_PACKET             =   0;       // data packet
};

packet RollingBallPacket extends RoutingPacket {
    int packetId;
    int RollingBallPacketKind enum (RollingBallPacketDef);
    int routingMode enum (RollingBallForwardingMode);
    Point destLocation;
    Point stuckLocation;
    Point ballCenter;
    Point previousLocation;
}



