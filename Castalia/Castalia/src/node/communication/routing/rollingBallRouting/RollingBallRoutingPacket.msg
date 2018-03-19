
cplusplus {{
#include "RoutingPacket_m.h"
#include "GeoMathHelper.h"
}}

class noncobject Point;

class RoutingPacket;


enum RollingBallPacketDef {
    ROLLINGBALL_DATA_PACKET             =   0;       // data packet
};

packet RollingBallPacket extends RoutingPacket {
    int packetId;
    int RollingBallPacketKind enum (RollingBallPacketDef);
    Point destLocation;
}



