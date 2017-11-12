
cplusplus {{
#include "RoutingPacket_m.h"
#include "GeoMathHelper.h"
}}

class noncobject Point;

class RoutingPacket;


enum GreedyPacketDef {
    GREEDY_DATA_PACKET             =   0;       // data packet
    GREEDY_HELLO_MSG_PACKET        =   1;      // hello msg
    GREEDY_REP_HELLO_MSG_PACKET    =   2;     // rep hello msg 
    GREEDY_SINK_ADDRESS_PACKET     =   3;    // sink address
}; 

packet GreedyPacket extends RoutingPacket {
    int packetId;
    int GreedyPacketKind enum (GreedyPacketDef);
    Point destLocation;

    // for hello message
    Point helloLocation;
}



