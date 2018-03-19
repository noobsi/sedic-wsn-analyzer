
cplusplus {{
#include "RoutingPacket_m.h"
#include "GeoMathHelper.h"
}}

class noncobject Point;

class RoutingPacket;

enum GpsrForwardingMode {
    GPSR_GREEDY_ROUTING = 0;
    GPSR_PERIMETER_ROUTING = 1;
};

enum GpsrPacketDef {
    GPSR_DATA_PACKET             =   0;       // data packet
    GPSR_HELLO_MSG_PACKET        =   1;      // hello msg
    GPSR_REP_HELLO_MSG_PACKET    =   2;     // rep hello msg 
    GPSR_SINK_ADDRESS_PACKET     =   3;    // sink address
}; 

packet GpsrPacket extends RoutingPacket {
    int packetId;
    int GpsrPacketKind enum (GpsrPacketDef);
    int routingMode enum (GpsrForwardingMode);
    int previousId;
    Point previousLocation; // to determine last edge
    Point destLocation; // Ld
    Point perimeterRoutingStartLocation; // Lp
    Point perimeterRoutingFaceLocation; // Lf
    int currentFaceFirstSender; // e0
    int currentFaceFirstReceiver; // e0
}



