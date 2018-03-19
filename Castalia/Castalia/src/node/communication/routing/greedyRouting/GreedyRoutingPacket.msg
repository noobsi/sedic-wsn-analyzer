
cplusplus {{
#include "RoutingPacket_m.h"
#include "GeoMathHelper.h"
}}

class noncobject Point;

class RoutingPacket;


enum GreedyPacketDef {
    GREEDY_DATA_PACKET             =   0;       // data packet
};

packet GreedyPacket extends RoutingPacket {
    int packetId;
    int GreedyPacketKind enum (GreedyPacketDef);
    Point destLocation;
}



