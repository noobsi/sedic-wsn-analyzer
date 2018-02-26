
cplusplus {{
#include "RoutingPacket_m.h"
#include "GeoMathHelper.h"
}}

class noncobject Point;

class RoutingPacket;


enum BoundholePacketDef {
    BOUNDHOLE_DATA_PACKET             =   0;       // data packet
    BOUNDHOLE_HELLO_MSG_PACKET        =   1;      // hello msg
    BOUNDHOLE_REP_HELLO_MSG_PACKET    =   2;     // rep hello msg
    BOUNDHOLE_SINK_ADDRESS_PACKET     =   3;    // sink address
}; 

packet BoundholePacket extends RoutingPacket {
    int packetId;
    int BoundholePacketKind enum (BoundholePacketDef);
    Point destLocation;

    // for hello message
    Point helloLocation;
}



